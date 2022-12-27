#pragma once

#include "types.h"

namespace jsonlite
{
  class Value
  {
  public:
    Value();
    
    Value(const Value& other);
    
    ~Value();

    template<typename T> bool is() const;

    template<typename T> void
    set(const T& value) {};

    void set(const String& value) {
      type = JsonType::STRING_;
      string_value = new String;
      *string_value = value;
    }

    void set(const Byte& value) {
      type = JsonType::NUMBER_;
      number_value._byte = static_cast<Byte>(value);
    }

    void set(const UByte& value) {
      type = JsonType::NUMBER_;
      number_value._ubyte = static_cast<UByte>(value);
    }

    void set(const Int16& value) {
      type = JsonType::NUMBER_;
      number_value._int_16 = static_cast<Int16>(value);
    }

    void set(const UInt16& value) {
      type = JsonType::NUMBER_;
      number_value._uint_16 = static_cast<UInt16>(value);
    }

    void set(const Int32& value) {
      type = JsonType::NUMBER_;
      number_value._int_32 = static_cast<Int32>(value);
    }

    void set(const UInt32& value) {
      type = JsonType::NUMBER_;
      number_value._uint_32 = static_cast<UInt32>(value);
    }

    void set(const Int64& value) {
      type = JsonType::NUMBER_;
      number_value._int_64 = static_cast<Int64>(value);
    }

    void set(const UInt64& value) {
      type = JsonType::NUMBER_;
      number_value._uint_64 = static_cast<UInt64>(value);
    }

    void set(const Double& value) {
      type = JsonType::NUMBER_;
      number_value._double = static_cast<Double>(value);
    }

    void set(const Boolean& value) {
      type = JsonType::BOOLEAN_;
      bool_value = value;
    }

    JsonType type;
    
    union
    {
      String* string_value;
      Number number_value;
      Boolean bool_value;
      Object* obj_value;
      Array* array_value;
    };
    
  private:
    /* Numeric type definition */
    template<typename T>
    struct IsByte
    {
      static const bool value = std::is_same<T, Byte>::value;
    };

    template<typename T>
    struct IsUByte
    {
      static const bool value = std::is_same<T, UByte>::value;
    };

    template<typename T>
    struct IsInt16
    {
      static const bool value = std::is_same<T, Int16>::value;
    };

    template<typename T>
    struct IsUInt16
    {
      static const bool value = std::is_same<T, UInt16>::value;
    };

    template<typename T>
    struct IsInt32
    {
      static const bool value = std::is_same<T, Int32>::value;
    };

    template<typename T>
    struct IsUInt32
    {
      static const bool value = std::is_same<T, UInt32>::value;
    };

    template<typename T>
    struct IsInt64
    {
      static const bool value = std::is_same<T, Int64>::value;
    };

    template<typename T>
    struct IsUInt64
    {
      static const bool value = std::is_same<T, UInt64>::value;
    };

    template<typename T>
    struct IsDouble
    {
      static const bool value = std::is_same<T, Double>::value;
    };

    /* Typename specialization for JSON's String type */
    template<class T>
    struct IsString
    {
      static const bool value = std::is_same<T, String>::value || std::is_same<T, String*>::value;
    };

    /* Typename specialization for JSON's Object type */
    template<class T>
    struct IsObject
    {
      static const bool value = std::is_same<T, Object>::value || std::is_same<T, Object*>::value;
    };

    /* Typename specialization for JSON's Array type */
    template<class T>
    struct IsArray
    {
      static const bool value = std::is_same<T, Array>::value || std::is_same<T, Array*>::value;
    };
  };
}
