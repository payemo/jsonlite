#include "value.h"
#include <vector>

namespace jsonlite
{
  class Array
  {
  public:
    Array();
    Array(const Value& value);
    Array(const Array& other);
    ~Array();

    size_t size() const;
    bool empty() const;

    typedef std::vector<Value*> container;

    void clear();

    bool parse(std::istream& input);
    bool parse(const std::string& input);

    void append(const Array& other);
    void append(const Value& value);

    template<typename T> T& get(UInt32 index);
    template<typename T> const T& get(UInt32 index) const;

  private:
    static bool parse(std::istream& input, Array& array);
    void internalAdd(const Array& other);
    void internalAdd(const Value& value);
    
    container values;
  };

  template<typename T>
  T& Array::get(UInt32 index) {
    JSONLITE_ASSERT(index < size());
    return values.at(index)->get<T>();
  }

  template<typename T>
  const T& Array::get(UInt32 index) const {
    JSONLITE_ASSERT(index < size());
    const Value* v = values.at(index);
    return v->get<T>();
  }
}
