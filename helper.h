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
      
      bool isNeg = false;
      char ch(0);
      // use for rolling back to initial position when something goes wrong.
      // That means that invalid input number will be skipped.
      std::streampos rollbackPos = input.tellg();
      std::string numStr;

      // check for sign
      ch = input.peek();
      if(!input.eof() && (ch == '-' || ch == '+')) {
	isNeg = (ch == '-');
	input.get(ch);

	// check for next digit right after passing a sign.
	// If no digit occurs right after a sign character
	// then input number got invalid.
	input >> std::ws;
	if(input.eof() || !std::isdigit(input.peek())) {
	  input.seekg(rollbackPos);
	  return false;
	}

	numStr += ch;
      }

      // loop through digits
      input.get(ch);
      while(!input.eof() && std::isdigit(ch)) {
	numStr += ch;
	input.get(ch);
      }

      input >> std::ws;
      if(input.eof() || (ch == ',' || ch == '}' || ch == ']')) {
	// For now, I assume we can be pretty sure that an integer value had been gotten,
	// so it's needed to be parsed to appropriate type
	// TODO: may be it can be remade more elegant.
	long double num = 0;
	if(isNeg) {
	  num = std::stoll(numStr);
	}
	else {
	  num = std::stoull(numStr);
	}
	//const auto num = isNeg ? std::stoll(numStr) : std::stoull(numStr);

	if(in_range<Byte>(num)) {
	  value._byte = static_cast<Byte>(num);
	}
	else if(in_range<UByte>(num)) {
	  value._ubyte = static_cast<UByte>(num);
	}
	else if(in_range<Int16>(num)) {
	  value._int_16 = static_cast<Int16>(num);
	}
	else if(in_range<UInt16>(num)) {
	  value._uint_16 = static_cast<UInt16>(num);
	}
	else if(in_range<Int32>(num)) {
	  value._int_32 = static_cast<Int32>(num);
	}
	else if(in_range<UInt32>(num)) {
	  value._uint_32 = static_cast<UInt32>(num);
	}
	else if(in_range<Int64>(num)) {
	  value._int_64 = static_cast<Int64>(num);
	}
	else if(in_range<UInt64>(num)) {
	  value._uint_64 = static_cast<UInt64>(num);
	}

	// A bit messy, but input should be returned as if number has been parsed recently,
	// so the next loop's condition will work fine.
	if(ch == ',' || ch == '}' || ch == ']') {
	  input.unget();
	}
	return true;
      }

      if(ch == '.') {
	numStr += ch;

	input >> std::ws;
	input.get(ch);
	// Garantee that the next character must be a digit, invalid input otherwise.
	if(input.eof() || !std::isdigit(ch)) {
	  input.seekg(rollbackPos);
	  return false;
	}

	while(!input.eof() && std::isdigit(ch)) {
	  numStr += ch;
	  input.get(ch);
	}

	// check for exponent part
	if(!input.eof() && (ch == 'E' || ch == 'e')) {
	  numStr += ch;
	  // Sign always MUST be after exponent code
	  if(input.peek() != '+' && input.peek() != '-') {
	    input.seekg(rollbackPos);
	    return false;
	  }
	  input.get(ch);
	  numStr += ch; // sign

	  // check for number afterwards.
	  if(input.eof() || !std::isdigit(input.peek())) {
	    input.seekg(rollbackPos);
	    return false;
	  }

	  // loop though digits
	  input.get(ch);
	  while(!input.eof() && std::isdigit(ch)) {
	    numStr += ch;
	    input.get(ch);
	  }
	}

	input >> std::ws;
	if(input.eof() || (ch == ',' || ch == '}' || ch == ']')) {
	  std::istringstream iss(numStr);
	  double result;
	  iss >> result;
	  value._double = result;

	  if(ch == ',' || ch == '}' || ch == ']') {
	    input.unget();
	  }
	  return true;
	}
      }

      input.seekg(rollbackPos);
      return false;
    }

    // inline bool parse(std::istream& input, Number& value) {
    //   input >> std::ws;

    //   std::streampos rollbackPos = input.tellg();
    //   // no exception will be thrown if error state flag bieng changed.
    //   input.exceptions(std::istream::goodbit);

    //   long double num;
    //   input >> num;

    //   if(input.fail()) {
    // 	input.clear();
    // 	input.seekg(rollbackPos);
    // 	return false;
    //   }
      
    //   // set number value
    //   if(in_range<Byte>(num)) {
    // 	value._byte = static_cast<Byte>(num);
    //   }
    //   else if(in_range<Int16>(num)) {
    // 	value._int_16 = static_cast<Int16>(num);
    //   }
    //   else if(in_range<UInt16>(num)) {
    // 	value._uint_16 = static_cast<UInt16>(num);
    //   }
    //   else if(in_range<Int32>(num)) {
    // 	value._int_32 = static_cast<Int32>(num);
    //   }
    //   else if(in_range<UInt32>(num)) {
    // 	value._uint_32 = static_cast<UInt32>(num);
    //   }
    //   else if(in_range<Int64>(num)) {
    // 	value._int_64 = static_cast<Int64>(num);
    //   }
    //   else if(in_range<UInt64>(num)) {
    // 	value._uint_64 = static_cast<UInt64>(num);
    //   }
    //   else if(in_range<Double>(num)) {
    // 	value._double = static_cast<Double>(num);
    //   }
      
    //   return true;
    // }

    template<typename T>
    bool in_range(long double num) {
      return std::numeric_limits<T>::min() <= num && num <= std::numeric_limits<T>::max();
    }
  }
}
