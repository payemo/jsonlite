#include "jsonlite.h"

#define TEST(...) JSONLITE_ASSERT(__VA_ARGS__);

using namespace std;
using namespace jsonlite;

int main() {
  {
    Value v;
    auto str = String("Eugene");
    v.set(str);
    TEST(v.is<String>());
    TEST(v.get<String>() == "Eugene");
  }
  {
    Value v;
    auto str = String("Eugene");
    v.set(str);
    TEST(v.is<String>());
    TEST(v.get<String>() == "eugene");
  }

  return 0;
}
