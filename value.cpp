#include "value.h"

namespace jsonlite
{
  Value::Value()
  {
  }

  Value::~Value()
  {
    clear();
  }

  Value::Value(const Value& other)
  {
    if(this != &other) {
      switch(other.type) {
      case JsonType::NUMBER_:
	set(other.number_value);
	break;
      case JsonType::STRING_:
	set(*other.string_value);
	break;
      case JsonType::BOOLEAN_:
	set(other.bool_value);
	break;
      case JsonType::ARRAY_:
	set(*other.array_value);
	break;
      case JsonType::OBJECT_:
	set(*other.obj_value);
	break;
      case JsonType::NULL_:
	set(Nullable());
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
    return (type == JsonType::STRING_ && string_value == nullptr) ||
      (type == JsonType::OBJECT_ && obj_value == nullptr) ||
      (type == JsonType::ARRAY_ && array_value == nullptr);
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
