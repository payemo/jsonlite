#include "array.h"

namespace jsonlite
{
  Array::Array() : values()
  {
  }

  Array::Array(const Value& value) {
    internalAdd(value);
  }

  Array::Array(const Array& other) {
    internalAdd(other);
  }

  Array::~Array() { clear(); }

  size_t Array::size() const { return values.size(); }
  bool Array::empty() const { return size() == 0; }

  void Array::clear() {
    container::iterator it = values.begin();
    for(; it != values.end(); ++it)
      delete *it;

    values.clear();
  }

  bool Array::parse(std::istream& input) {
    return parse(input, *this);
  }

  bool Array::parse(const std::string& input) {
    std::istringstream iss(input);
    return parse(iss, *this);
  }

  void Array::append(const Array& other) {
    if(this != &other) {
      values.push_back(new Value(other));
    }
    else {
      internalAdd(Array(*this));
    }
  }

  void Array::append(const Value& value) {
    internalAdd(value);
  }

  bool Array::parse(std::istream& input, Array& array) {
    if(!helper::match(input, "[")) {
      return false;
    }
    if(helper::match(input, "]")) {
      return true;
    }

    do {
      Value* v = new Value;

      if(!v->parse(input)) {
	delete v;
	break;
      }

      array.values.push_back(v);
    } while(helper::match(input, ","));

    return helper::match(input, "]");
  }

  void Array::internalAdd(const Array& other) {
    if(this != &other) {
      container::const_iterator it = other.values.begin();
      for(; it != other.values.end(); ++it) {
	values.push_back(new Value(**it));
      }
    }
    else {
      internalAdd(Array(*this));
    }
  }

  void Array::internalAdd(const Value& value) {
    values.push_back(new Value(value));
  }
}
