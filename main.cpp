#include "jsonlite.h"
#include <sstream>

#define TEST(...) JSONLITE_ASSERT(__VA_ARGS__);

using namespace std;
using namespace jsonlite;

int main() {
	{
		Value v;
		string test_str("\"\\u000e\\u0002\"");
		stringstream input(test_str);
		TEST(v.parse(input));
		TEST(v.is<String>());
		TEST(v.get<String>() == "\xe\x2");
	}
	{
		Value v;
		string test_str("\"test_key\"");
		stringstream input(test_str);
		TEST(v.parse(input));
		TEST(v.get<String>() == "test_key");
	}
	{
		Value v;
		string test_str("\"\"");
		stringstream input(test_str);
		TEST(v.parse(input));
		TEST(v.is<String>() && v.get<String>() == "");
	}
	{
		Value v;
		string str("test_key");
		stringstream input(str);
		v.parse(input);
		TEST(v.empty());
	}
	// test numbers
	{
		string teststr("-6");
		stringstream input(teststr);
		Value v;
		TEST(v.parse(input));
		TEST(v.get<Number>() == -6);
	}
	{
		string teststr("1023.1929254849277");
		stringstream input(teststr);
		Value v;
		TEST(v.parse(input));
		TEST(v.get<Number>() == 1023.1929254849277);
	}
	{
		string teststr("3.402823E+38");
		stringstream input(teststr);
		Value v;
		TEST(v.parse(input));
		TEST(v.get<Number>() == 3.402823E+38);
	}
	{
		string teststr("4.2e+7");
		stringstream input(teststr);
		Value v;
		TEST(v.parse(input));
		TEST(v.get<Number>() == 42000000);
	}
	{
		string teststr("4.2e+,");
		stringstream input(teststr);
		Value v;
		v.parse(input);
		TEST(v.empty());
	}
	// parse boolean
	{
		string teststr("true");
		stringstream input(teststr);
		Value v;
		TEST(v.parse(input));
		TEST(v.get<Boolean>() == true);
	}
	{
		string teststr("\ttrue   ");
		stringstream input(teststr);
		Value v;
		TEST(v.parse(input));
		TEST(v.get<Boolean>() == true);
	}
	// test object
	{
		string teststr(
				"{"
				"  \"foo\" : 1,"
				"  \"bar\" : false,"
				"  \"person\" : {\"name\" : \"GWB\", \"age\" : 60},"
				"}"
		);
		istringstream input(teststr);
		Object o;
		TEST(o.parse(input));
		TEST(1 == o.get<Number>("foo"));
		TEST(o.has<Boolean>("bar"));
		TEST(o.has<Object>("person"));
		TEST(o.get<Object>("person").has<Number>("age"));
		TEST(!o.has<Number>("data"));
	}
	return 0;
}
