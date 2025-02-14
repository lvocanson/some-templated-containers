#pragma once
#include <iostream>
#include <vector>

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#ifdef TESTS_ENABLED

typedef void (*function_signature)();
struct test
{
	function_signature function;
	const char* name;
};

inline std::vector<test>& get_tests()
{
	static std::vector<test> tests;
	return tests;
}

#define CREATE_TEST(name) \
static void CONCAT(name, __LINE__)(); \
static bool CONCAT(name, __LINE__)##_is_registered = []()\
{ \
get_tests().push_back({CONCAT(name, __LINE__), #name});\
return true; \
}(); \
static void CONCAT(name, __LINE__)()

#define LINE_STR_IMPL_IMPL(line) #line
#define LINE_STR_IMPL(line) LINE_STR_IMPL_IMPL(line)
#define LINE_STR LINE_STR_IMPL(__LINE__)

#define CHECK(condition) \
if (!(condition)) \
{ \
	throw std::exception("The condition `" #condition "` did"\
	" not evaluate to true. (" __FILE__ ":" LINE_STR ")");\
}

#define TRY_EXCEPTION(function_call, expected_exception)\
try \
{ \
	function_call; \
	throw std::exception("The function call `" #function_call\
	" did not throw an exception of type `" #expected_exception\
	"`. (" __FILE__ ":" LINE_STR ")"); \
} \
catch (const expected_exception&) {}

inline void execute_tests()
{
	std::cout << "Tests found: " << get_tests().size() << "\n\n";
	size_t passed = 0;
	for (auto& test : get_tests())
	{
		std::cout << "Executing test `" << test.name << "`... ";
		try
		{
			test.function();
			std::cout << "passed.\n";
			++passed;
		}
		catch (const std::exception& e)
		{
			std::cout << "failed. Exception: " << e.what() << '\n';
		}
		catch (...)
		{
			std::cout << "failed. Unknown error.\n";
		}
	}
	std::cout << "\nResults: " << passed << '/' << get_tests().size() << " passed.\n";
}

#else // TESTS_ENABLED

#define CREATE_TEST(name) static void CONCAT(name, __LINE__)()
#define CHECK(condition)
#define TRY_EXCEPTION(function_call, expected_exception)
inline void execute_tests() {}

#endif // TESTS_ENABLED
