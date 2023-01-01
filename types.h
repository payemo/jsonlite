#pragma once
#include <string>
#include <type_traits>

namespace jsonlite
{
  enum class JsonType { STRING_, NUMBER_, OBJECT_, ARRAY_, BOOLEAN_, NULL_, INVALID_ };

  typedef char Byte;
  typedef unsigned char UByte;
  typedef int Int16;
  typedef unsigned int UInt16;
  typedef long int Int32;
  typedef unsigned long int UInt32;
  typedef long long int Int64;
  typedef unsigned long long int UInt64;
  typedef double Double;
  
  // union _number
  // {
  //   Byte _byte;
  //   UByte _ubyte;
  //   Int16 _int_16;
  //   UInt16 _uint_16;
  //   Int32 _int_32;
  //   UInt32 _uint_32;
  //   Int64 _int_64;
  //   UInt64 _uint_64;
  //   Double _double;
  // };
  
  /* Fast-forwarding defintion of types */
  typedef std::string String;
  typedef long double Number;
  typedef bool Boolean;
  class Object;
  class Array;
  class Nullable{};

  // TODO
  /* Exception types */
  //class JsonTypeException : std::runtime_exception {};
}
