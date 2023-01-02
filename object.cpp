#include "object.h"

namespace jsonlite
{
  Object::Object() : values() {}
  
  Object::~Object() { clear(); }

  Object::Object(const Object& other) {
    set(other);
  }
  
  Object::Object(const std::string key, const Value& value) {
    set(key, value);
  }

  void Object::clear() {
    container::iterator it;
    for(it = values.begin(); it != values.end(); ++it) {
      delete it->second;
    }
    values.clear();
  }

  bool Object::parse(std::istream& input) {
    return parse(input, *this);
  }

  bool Object::parse(const std::string& input) {
    std::istringstream iss(input);
    return parse(iss, *this);
  }

  bool Object::empty() const { return values.empty(); }

  bool Object::trySet(const std::string key, Value& value) {
    if(values.count(key) == 0) {
      values[key] = &value;
      return true;
    }
    return false;
  }

  bool Object::parse(std::istream& input, Object& object) {
    if(!helper::match(input, "{")) {
      return false;
    }
    if(helper::match(input, "}")) {
      return true;
    }

    do {
      String key;
      if(!helper::parse(input, key)) {
	return false;
      }

      if(!helper::match(input, ":")) {
	return false;
      }

      Value* v = new Value;
      if(!v->parse(input)) {
	delete v;
	break;
      }

      if(!object.trySet(key, *v)) {
	delete v;
	return false;
      }
    } while(helper::match(input, ","));

    return helper::match(input, "}");
  }

  void Object::set(const Object& other) {
    osKey.clear();
    if(this != &other) {
      container::const_iterator it = other.values.begin();
      for(; it != other.values.end(); ++it) {
	auto found = values.find(it->first);
	if(found != values.end()) {
	  delete found->second;
	}
	values[it->first] = new Value(*(it->second));
      }
    }
    else {
      set(Object(*this));
    }
  }

  void Object::set(const std::string& key, const Value& value) {
    osKey.clear();
    auto found = values.find(key);
    if(found != values.end()) {
      delete found->second;
    }
    values[key] = new Value(value);
  }

  Object& Object::operator <<(const Value& value) {
    if(osKey.empty() && value.is<String>()) {
      osKey = value.get<String>();
    }
    else {
      set(Object(osKey, value));
      osKey.clear();
    }
    return *this;
  }

  Object& Object::operator <<(const Object& value) {
    set(Object(std::string(osKey), value));
    osKey.clear();
    return *this;
  }

  Object& Object::operator =(const Object& value) {
    osKey.clear();
    if(this != &value) {
      clear();
      set(value);
    }
    return *this;
  }

  Object::constIterator Object::beginValues() const { return values.begin(); }
  Object::constIterator Object::endValues() const { return values.end(); }

  std::ostream& operator <<(std::ostream& os, const Object& value) {
    using namespace jsonlite;
    using namespace helper;
    
    os << '}';
    auto beg = value.beginValues(), end = value.endValues();
    for(; beg != end;) {
      streamString(os, beg->first);
      os << ": " << *beg->second;

      ++beg;
      if(beg != end)
	os << ", ";
    }
    return os << "}";
  }
}
