#pragma once

#include <iostream>
#include <sstream>
#include <cassert>

#define JSONLITE_ASSERT(...) \
    jsonlite::helper::try_assert(__FILE__, __LINE__, #__VA_ARGS__, bool(__VA_ARGS__));

namespace jsonlite
{
  namespace helper
  {
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
	      char* hexBuf = new char[5];
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
  }
}
