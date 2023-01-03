#pragma once

#include "types.h"
#include "helper.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>

namespace jsonlite
{
  class Value
  {
  public:
    Value();
    
    Value(const Value& other);

    template<typename T>
    Value(const T& value) : type(JsonType::INVALID_) { set(value); }
    
    ~Value();

    bool parse(std::istream& stream);
    bool parse(const std::string& input);

    void clear();

    bool empty() const;

    template<typename T> bool is() const;
    template<typename T> T& get();
    template<typename T> const T& get() const;

    template<typename T>
    void set(const T& t) {};
    
    void set(const String& value);
    void set(const char* value);
    void set(const Number& value);
    void set(const Boolean& value);
    void set(const Array& value);
    void set(const Object& value);
    void set(const Value& value);
    void set(const Nullable& value);
    void set(const std::nullptr_t& value);

    std::string getJsonString(int depth, const Value& value) const;

    JsonType type;

    // operators overloading
    template<typename T>
    Value& operator <<(const T& value) {
      set(value);
      return *this;
    }

    template<typename T>
    Value& operator =(const Value& value) {
      set(value);
      return *this;
    }

    friend std::ostream& operator <<(std::ostream& os, const Value& value);

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
