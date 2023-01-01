#include "value.h"
#include "object.h"

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
      switch(other.type) {
      case JsonType::NUMBER_:
	type = JsonType::NUMBER_;
	number_value = other.number_value;
	break;
      case JsonType::STRING_:
	type = JsonType::STRING_;
	string_value = new String;
	string_value = other.string_value;
	break;
      case JsonType::BOOLEAN_:
	type = JsonType::BOOLEAN_;
	bool_value = other.bool_value;
	break;
      case JsonType::ARRAY_:
	// type = JsonType::ARRAY_;
	// array_value = new Array;
	// array_value = other.array_value;
	break;
      case JsonType::OBJECT_:
	type = JsonType::OBJECT_;
	obj_value = new Object;
	obj_value = other.obj_value;
	break;
      case JsonType::NULL_:
	type = JsonType::NULL_;
	break;
      }
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

    value.obj_value = new Object;
    if(value.obj_value->parse(input)) {
      value.type = JsonType::OBJECT_;
      return true;
    }

    delete value.obj_value;
    value.obj_value = nullptr;
    return false;
  }
  
  // template<typename T>
  // void Value::set(const T& value) {
  //   if(IsByte<T>::value) {
  //     type = JsonType::NUMBER_;
  //     number_value._byte = static_cast<Byte>(value);
  //   }
  //   else if(IsUByte<T>::value) {
  //     type = JsonType::NUMBER_;;
  //     number_value._ubyte = static_cast<UByte>(value);
  //   }
  //   else if(IsInt16<T>::value) {
  //     type = JsonType::NUMBER_;
  //     number_value._int_16 = static_cast<Int16>(value);
  //   }
  //   else if(IsUInt16<T>::value) {
  //     type = JsonType::NUMBER_;
  //     number_value._uint_16 = static_cast<UInt16>(value);
  //   }
  //   else if(IsInt32<T>::value) {
  //     type = JsonType::NUMBER_;
  //     number_value._int_32 = static_cast<Int32>(value);
  //   }
  //   else if(IsUInt32<T>::value) {
  //     type = JsonType::NUMBER_;
  //     number_value._uint_32 = static_cast<UInt32>(value);
  //   }
  //   else if(IsInt64<T>::value) {
  //     type = JsonType::NUMBER_;
  //     number_value._int_64 = static_cast<Int64>(value);
  //   }
  //   else if(IsUInt64<T>::value) {
  //     type = JsonType::NUMBER_;
  //     number_value._uint_64 = static_cast<UInt64>(value);
  //   }
  //   else if(IsDouble<T>::value) {
  //     type = JsonType::NUMBER_;
  //     number_value._double = static_cast<Double>(value);
  //   }
  //   else if(IsString<T>::value) {
  //     type = JsonType::STRING_; 
  //     string_value = new String;
  //     *string_value = value;
  //   }
  //   // else if(IsObject<T>::value) {
  //   //   type = JsonType::OBJECT_;
  //   //   obj_value = new Object;
  //   //   *obj_value = value;
  //   // }
  //   // else if(IsArray<T>::value) {
  //   //   type = JsonType::ARRAY_;
  //   //   array_value = new Array;
  //   //   *array_value = value;
  //   // }
  //   else {
  //     // TODO: add custom exceptions
  //     //throw ValueTypeException{};
  //   }
  // }
}
