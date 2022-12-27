#include "value.h"

namespace jsonlite
{
  Value::Value()
  {
  }

  Value::~Value()
  {
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
  
  /* Numeric JSON types. */
  template<>
  inline bool Value::is<Number>() const { return type == JsonType::NUMBER_; }

  /* Boolean JSON type. */
  template<>
  inline bool Value::is<bool>() const { return type == JsonType::BOOLEAN_; }

  /* String JSON type. */
  template<>
  inline bool Value::is<String>() const { return type == JsonType::STRING_; }

  /* Object JSON type. */
  template<>
  inline bool Value::is<Object>() const { return type == JsonType::OBJECT_; }

  /* Array JSON type. */
  template<>
  inline bool Value::is<Array>() const { return type == JsonType::ARRAY_; }

  /* Null JSON type. */
  template<>
  inline bool Value::is<Nullable>() const { return type == JsonType::NULL_; }
  
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
