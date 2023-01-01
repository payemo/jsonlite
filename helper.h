#pragma once

#include <iostream>
#include <sstream>
#include <cassert>
#include <limits>
#include <cctype>
#include <string>

#define JSONLITE_ASSERT(...) \
    jsonlite::helper::try_assert(__FILE__, __LINE__, #__VA_ARGS__, bool(__VA_ARGS__));

namespace jsonlite
{
  namespace helper
  {
    template<typename T>
	bool in_range(long double n);
    
    inline void try_assert(const char *file, int line, const char *expression, bool result) {
      if(!result) {
	std::cerr << "Expression: " << expression << " failed at: " << file << ": " << line << std::endl;
      }
    }

    inline bool match(std::istream& input, const char* pattern) {
      // discard leading whitespace from an input stream.
      input >> std::ws;
      const char* curr = pattern;
      char ch;

      while(!input.eof() && *curr != '\0') {
	input.get(ch);

	if(ch == *curr) {
	  curr++;
	}
	else { // rollback everything to initial state

	  /* If matching process being stopped on the first character
	  then just put back taken character from stream and return false.
	  Otherwise, rollback current pointer to initial pattern. */
	  if(curr == pattern) {
	    input.putback(ch);
	  }
	  else {
	    while(curr > pattern) {
	      input.putback(*curr);
	      --curr;
	    }
	  }

	  return false;
	}
      }

      return *curr == '\0';
    }

    inline bool parse(std::istream& input, String& value) {
      char ch(0);
      // check if the first sequence of characters mathes a double quote.
      if(!match(input, "\"")) {
	return false;
      }

      while(input.get(ch)) {
	if(ch == '"') {
	  break;
	}

	if(ch == '\\') {
	  input.get(ch);
	  
	  switch(ch) {
	  case '\\':
	  case '/':
	    value += ch;
	    break;
	  case 'b':
	    value += '\b';
	    break;
	  case 'f':
	    value += '\f';
	    break;
	  case 'n':
	    value += '\n';
	    break;
	  case 'r':
	    value += '\r';
	    break;
	  case 't':
	    value += '\t';
	    break;
	  case 'u':
	    {
	      char* hexBuf = new char[4];
	      std::stringstream ss{hexBuf};
	      if(input.read(hexBuf, 4)) {
		ss << std::hex << hexBuf;
	      }

	      if(input.good()) {
		int hexNum;
		ss >> hexNum;
		value += hexNum;
	      }

	      delete[] hexBuf;
	    }
	    break;
	  default:
	    if(ch != '"') {
	      value += ('\\' + ch);
	    }
	    else {
	      value += ch;
	    }
	  }
	}
	else {
	  value += ch;
	}
      }
      
      return (!input.eof() && input.good() && ch == '"');
    }

    inline bool parse(std::istream& input, Number& value) {
      input >> std::ws;
      
      char ch(0);
      char lastCh(0);
      // use for rolling back to initial position when something goes wrong.
      // That means that invalid input number will be skipped.
      std::streampos rollbackPos = input.tellg();
      std::string numStr;

      // check for a sign
      if(!input.eof() && (input.peek() == '-' || input.peek() == '+')) {
	input.get(ch);
	numStr += ch;
	lastCh = ch;
      }
      
      while(!input.eof() && std::isdigit((ch = input.peek()))) {
	input.get(ch);
	numStr += ch;
	lastCh = ch;
      }

      input >> std::ws;
      
      if(ch == '.') {
	numStr += ch;
	input.get(ch); // retrieve current input byte.
	lastCh = ch;

	input >> std::ws;

	while(!input.eof() && std::isdigit((ch = input.peek()))) {
	  input.get(ch);
	  numStr += ch;
	  lastCh = ch;
	}

	// check for exponent part
	input.get(ch); //retrieve
	char expSign = input.peek();
	
	if(!input.eof() && (ch == 'E' || ch == 'e') && (expSign == '+' || expSign == '-')) {
	  numStr += ch;
	  numStr += expSign; // sign
	  input.get(ch); // ch = sign, so we can take next character from stream
	  lastCh = ch;

	  while(!input.eof() && std::isdigit((ch = input.peek()))) {
	    input.get(ch);
	    numStr += ch;
	    lastCh = ch;
	  }
	}
	else {
	  input.putback(ch);
	}
      }

      input >> std::ws;
      // we need to be sure that by parsing a number, the last character must be a digit.
      if(input.eof() || std::isdigit(lastCh)) {
	value = std::stold(numStr);
	return true;
      }
      else {
	input.seekg(rollbackPos);
	return false;
      }
    }

    inline bool parse(std::istream& input, Boolean& value) {
      if(match(input, "true")) {
	value = true;
	return true;
      }
      else if(match(input, "false")) {
	value = false;
	return true;
      }
      else {
	return false;
      }
    }

    inline bool parse(std::istream& input) {
      return match(input, "null");
    }

    template<typename T>
    bool in_range(long double num) {
      return std::numeric_limits<T>::min() <= num && num <= std::numeric_limits<T>::max();
    }
  }
}
