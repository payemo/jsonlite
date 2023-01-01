#include "value.h"
#include "object.h"
#include "array.h"

namespace jsonlite
{
  Value::Value() : type(JsonType::INVALID_)
  {
  }

  Value::~Value()
  {
    clear();
  }

  Value::Value(const Value& other) : type(JsonType::INVALID_)
  {
    clear();
    if(this != &other) {
      set(other);
    }
  }

  void Value::clear() {
    if(type == JsonType::STRING_) {
      delete string_value;
      string_value = nullptr;
    }
    else if(type == JsonType::OBJECT_) {
      delete obj_value;
      obj_value = nullptr;
    }
    else if(type == JsonType::ARRAY_) {
      delete array_value;
      array_value = nullptr;
    }
  }

  bool Value::empty() const
  {
    if(type == JsonType::INVALID_) return true;
    if(type == JsonType::STRING_ && string_value == nullptr) return true;
    if(type == JsonType::OBJECT_ && obj_value == nullptr) return true;
    if(type == JsonType::ARRAY_ && array_value == nullptr) return true;
    return false;
  }

  bool Value::parse(std::istream& input) {
    return parse(input, *this);
  }

  bool Value::parse(const std::string& input) {
    std::istringstream iss(input);
    return parse(iss, *this);
  }

  bool Value::parse(std::istream& input, Value& value) {
    String stringValue;
    if(helper::parse(input, stringValue)) {
      value.type = JsonType::STRING_;
      value.string_value = new String;
      value.string_value->swap(stringValue);
      return true;
    }

    Number numValue;
    if(helper::parse(input, numValue)) {
      value.type = JsonType::NUMBER_;
      value.number_value = numValue;
      return true;
    }

    Boolean boolValue;
    if(helper::parse(input, boolValue)) {
      value.type = JsonType::BOOLEAN_;
      value.bool_value = boolValue;
      return true;
    }

    if(helper::parse(input)) {
      value.type = JsonType::NULL_;
      return true;
    }

    if(input.peek() == '[') {
      value.array_value = new Array;
      if(value.array_value->parse(input)) {
	value.type = JsonType::ARRAY_;
	return true;
      }

      delete value.array_value;
      value.array_value = nullptr;
    }

    value.obj_value = new Object;
    if(value.obj_value->parse(input)) {
      value.type = JsonType::OBJECT_;
      return true;
    }

    delete value.obj_value;
    value.obj_value = nullptr;
    return false;
  }

  void Value::set(const String& value) {
    type = JsonType::STRING_;
    string_value = new String;
    *string_value = value;
  }

  void Value::set(const Number& value) {
    type = JsonType::NUMBER_;
    number_value = value;
  }

  void Value::set(const Boolean& value) {
    type = JsonType::BOOLEAN_;
    bool_value = value;
  }

  void Value::set(const Array& value) {
    type = JsonType::ARRAY_;
    array_value = new Array;
    *array_value = value;
  }

  void Value::set(const Object& value) {
    type = JsonType::OBJECT_;
    obj_value = new Object;
    *obj_value = value;
  }

  void Value::set(const Value& value) {
    switch(value.type) {
    case JsonType::NUMBER_:
      set(value.number_value);
      break;
    case JsonType::STRING_:
      set(*value.string_value);
      break;
    case JsonType::BOOLEAN_:
      set(value.bool_value);
      break;
    case JsonType::ARRAY_:
      set(*value.array_value);
      break;
    case JsonType::OBJECT_:
      set(*value.obj_value);
      break;
    case JsonType::NULL_:
      type = JsonType::NULL_;
      break;
    default:
      type = JsonType::INVALID_;
      break;
    }
  }
}
