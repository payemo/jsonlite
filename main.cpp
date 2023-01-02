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
				"  \"data\": [\"abcd\", 42, 54.7]"
				"}"
		);
		istringstream input(teststr);
		Object o;
		TEST(o.parse(input));
		TEST(1 == o.get<Number>("foo"));
		TEST(o.has<Boolean>("bar"));
		TEST(o.has<Object>("person"));
		TEST(o.get<Object>("person").has<Number>("age"));
		TEST(o.has<Array>("data"));
		TEST(o.get<Array>("data").get<Number>(1) == 42);
		TEST(o.get<Array>("data").get<String>(0) == "abcd");
		TEST(o.get<Array>("data").get<Number>(2) - 54.7 < 1e-6 ||
				-o.get<Array>("data").get<Number>(2) + 54.7 < 1e-6);
		TEST(!o.has<Number>("data"));
	}
	{
		string teststr = R"(
		{
			"fruit": "Apple",
			"size": "Large",
			"color": "Red"
		}
		)";
		istringstream input(teststr);
		Object o;
		TEST(o.parse(input));
	}
	{
		string teststr = R"(
		{
			"quiz": {
				"sport": {
					"q1": {
						"question": "Which one is correct team name in NBA?",
						"options": [
							"New York Bulls",
							"Los Angeles Kings",
							"Golden State Warriros",
							"Huston Rocket"
						],
						"answer": "Huston Rocket"
					}
				},
				"maths": {
					"q1": {
						"question": "5 + 7 = ?",
						"options": [
							"10",
							"11",
							"12",
							"13"
						],
						"answer": "12"
					},
					"q2": {
						"question": "12 - 8 = ?",
						"options": [
							"1",
							"2",
							"3",
							"4"
						],
						"answer": "4"
					}
				}
			}
		}
		)";
		istringstream input(teststr);
		Object o;
		TEST(o.parse(input));
	}
	{
		Array a;
		a << Number(123);
		a << String("hello world");
		a << Number(3.1415);
		a << Number(99.95f);
		Object o;
		o << String("key1") << String("value");
		a << o;
	}
	// TODO: figure out why value is not set using that way
	{
		Array a;
		a << 123;
		a << "hello world";
		a << 3.1415;
		a << 99.95f;
		a << 'h';
		a << Object("key", "value");
	}
	return 0;
}
