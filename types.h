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
  
  /* Fast-forwarding defintion of types */
  typedef std::string String;
  typedef long double Number;
  typedef bool Boolean;
  class Object;
  class Array;
  class Nullable{};
}
