#include "jsonlite.h"
#include <sstream>

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
		string test_str("\"\\u000e\\u0002\"");
		string result;
		stringstream input(test_str);
		TEST(helper::parse(input, result));
		TEST(result == "\xe\x2");
	}
	{
		Value v;
		string test_str("\"test_key\"");
		string result;
		stringstream input(test_str);
		TEST(helper::parse(input, result));
		TEST(result == "test_key");
	}
	{
		Value v;
		string test_str("\"\"");
		string result;
		stringstream input(test_str);
		TEST(helper::parse(input, result));
	}
	{
		Value v;
		string str("test_key");
		string res;
		stringstream input(str);

		TEST(helper::parse(input, res));
		TEST(res == "");
	}
	// test numbers
	{
		string teststr("-6");
		stringstream input(teststr);
		Value v;
		Number n;
		TEST(helper::parse(input, n));
		TEST(n == -6);
	}
	{
		string teststr("1023.1929254849277");
		stringstream input(teststr);
		Value v;
		Number n;
		TEST(helper::parse(input, n));
		TEST(n == 1023.1929254849277);
	}
	{
		string teststr("3.402823E+38");
		stringstream input(teststr);
		Value v;
		Number n;
		TEST(helper::parse(input, n));
		TEST(n == 3.402823E+38);
	}
	{
		string teststr("4.2e+7");
		stringstream input(teststr);
		Value v;
		Number n;
		TEST(helper::parse(input, n));
		TEST(n == 42000000);
	}
	{
		string teststr("4.2e+,");
		stringstream input(teststr);
		Value v;
		Number n;
		TEST(helper::parse(input, n));
		TEST(n == 42000000);
	}
	return 0;
}
