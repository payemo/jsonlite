#pragma once

#include "types.h"
#include "helper.h"
#include <iostream>

namespace jsonlite
{
  class Value
  {
  public:
    Value();
    
    Value(const Value& other);
    
    ~Value();

    bool parse(std::istream& stream);
    bool parse(const std::string& input);

    void clear();

    bool empty() const;

    template<typename T> bool is() const;
    template<typename T> T& get();
    template<typename T> const T& get() const;

    // void set(const String& value) {
    //   type = JsonType::STRING_;
    //   string_value = new String;
    //   *string_value = value;
    // }

    // void set(const Number& value) {
    //   type = JsonType::NUMBER_;
    //   number_value = value;
    // }

    // void set(const Byte& value) {
    //   type = JsonType::NUMBER_;
    //   number_value._byte = static_cast<Byte>(value);
    // }

    // void set(const UByte& value) {
    //   type = JsonType::NUMBER_;
    //   number_value._ubyte = static_cast<UByte>(value);
    // }

    // void set(const Int16& value) {
    //   type = JsonType::NUMBER_;
    //   number_value._int_16 = static_cast<Int16>(value);
    // }

    // void set(const UInt16& value) {
    //   type = JsonType::NUMBER_;
    //   number_value._uint_16 = static_cast<UInt16>(value);
    // }

    // void set(const Int32& value) {
    //   type = JsonType::NUMBER_;
    //   number_value._int_32 = static_cast<Int32>(value);
    // }

    // void set(const UInt32& value) {
    //   type = JsonType::NUMBER_;
    //   number_value._uint_32 = static_cast<UInt32>(value);
    // }

    // void set(const Int64& value) {
    //   type = JsonType::NUMBER_;
    //   number_value._int_64 = static_cast<Int64>(value);
    // }

    // void set(const UInt64& value) {
    //   type = JsonType::NUMBER_;
    //   number_value._uint_64 = static_cast<UInt64>(value);
    // }

    // void set(const Double& value) {
    //   type = JsonType::NUMBER_;
    //   number_value._double = static_cast<Double>(value);
    // }

    // void set(const Boolean& value) {
    //   type = JsonType::BOOLEAN_;
    //   bool_value = value;
    // }

    /*void set(const Object& value) {
      type = JsonType::OBJECT_;
      obj_value = new Object;
      *obj_value = value;
    }*/

    JsonType type;

  private:
    union
    {
      String* string_value;
      Number number_value;
      Boolean bool_value;
      Object* obj_value;
      Array* array_value;
    };
    
    static bool parse(std::istream& input, Value& value);
  };

  /* Numeric JSON types. */
  template<>
  inline bool Value::is<Number>() const { return type == JsonType::NUMBER_; }

  /* Boolean JSON type. */
  template<>
  inline bool Value::is<Boolean>() const { return type == JsonType::BOOLEAN_; }

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

  template<>
  inline Value& Value::get<Value>() { return *this; }

  template<>
  inline const Value& Value::get<Value>() const { return *this; }

  template<>
  inline Number& Value::get<Number>() {
      JSONLITE_ASSERT(is<Number>());
      return number_value;
  }

  template<>
  inline const Number& Value::get<Number>() const {
      JSONLITE_ASSERT(is<Number>());
      return number_value;
  }

  template<>
  inline String& Value::get<String>() {
      JSONLITE_ASSERT(is<String>());
      return *string_value;
  }

  template<>
  inline const String& Value::get<String>() const {
      JSONLITE_ASSERT(is<String>());
      return *string_value;
  }

  template<>
  inline Boolean& Value::get<Boolean>() {
      JSONLITE_ASSERT(is<Boolean>());
      return bool_value;
  }

  template<>
  inline const Boolean& Value::get<Boolean>() const {
      JSONLITE_ASSERT(is<Boolean>());
      return bool_value;
  }

  template<>
  inline Array& Value::get<Array>() {
      JSONLITE_ASSERT(is<Array>());
      return *array_value;
  }

  template<>
  inline const Array& Value::get<Array>() const {
      JSONLITE_ASSERT(is<Array>());
      return *array_value;
  }

  template<>
  inline Object& Value::get<Object>() {
      JSONLITE_ASSERT(is<Object>());
      return *obj_value;
  }

  template<>
  inline const Object& Value::get<Object>() const {
      JSONLITE_ASSERT(is<Object>());
      return *obj_value;
  }
}
