#pragma once

#include "types.h"
#include "value.h"
#include <map>

namespace jsonlite
{
  class Object
  {
  public:
    Object();
    Object(const std::string key, const Value& value);
    Object(const Object& other);
    ~Object();

    typedef std::map<std::string, Value*> container;

    void clear();
    
    bool parse(std::istream& input);

    bool parse(const std::string& input);

    bool empty() const;

    bool trySet(std::string key, Value& value);
    
    template<typename T> bool has(const std::string key) const;

    template<typename T> T& get(const std::string key) const;
  private:
    container values;

    static bool parse(std::istream& input, Object& object);
  };

  template<typename T>
  bool Object::has(const std::string key) const {
    container::const_iterator it = values.find(key);
    return it != values.end() && it->second->is<T>();
  }

  template<typename T>
  T& Object::get(const std::string key) const {
    JSONLITE_ASSERT(has<T>(key));
    return values.find(key)->second->get<T>();
  }
}
