#pragma once
#include <iostream>
#include <vector>

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

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
static volatile bool CONCAT(name, __LINE__)##_is_registered = []() \
{ \
get_tests().push_back({CONCAT(name, __LINE__), #name}); \
return true; \
}(); \
static void CONCAT(name, __LINE__)()

#define LINE_STR_IMPL_IMPL(line) #line
#define LINE_STR_IMPL(line) LINE_STR_IMPL_IMPL(line)
#define LINE_STR LINE_STR_IMPL(__LINE__)

struct check_failed_exception
{
	const char* condition;
	const char* file;
	const char* line;
};

#define CHECK(condition) \
if (!(condition)) \
{ \
	throw check_failed_exception{#condition, __FILE__, LINE_STR}; \
}

struct try_exception_failed_exception
{
	const char* function_call;
	const char* expected;
	const char* file;
	const char* line;
};

struct try_exception_wrong_exception
{
	const char* function_call;
	const char* expected;
	const char* got;
	const char* file;
	const char* line;
};

#define TRY_EXCEPTION(function_call, expected_exception) \
try \
{ \
	function_call; \
	throw try_exception_failed_exception{#function_call, #expected_exception, __FILE__, LINE_STR}; \
} \
catch (const expected_exception&) {} \
catch (const std::exception& e) \
{ \
	throw try_exception_wrong_exception{#function_call, #expected_exception, e.what(), __FILE__, LINE_STR}; \
}

inline void execute_tests()
{
	std::cout << "Tests found: " << get_tests().size() << "\n\n";
	size_t passed = 0;
	for (auto& test : get_tests())
	{
		std::cout << "Testing `" << test.name << "`... ";
		try
		{
			test.function();
			std::cout << "OK.\n";
			++passed;
		}
		catch (const check_failed_exception& e)
		{
			std::cout
				<< "\n> CHECK failed: " << e.condition
				<< "\n> At line: " << e.line
				<< "\n> Of file: " << e.file << '\n';
		}
		catch (const try_exception_failed_exception& e)
		{
			std::cout
				<< "\n> TRY_EXCEPTION failed: " << e.function_call
				<< "\n> Did not throw."
				<< "\n> Expected: " << e.expected
				<< "\n> At line: " << e.line
				<< "\n> Of file: " << e.file << '\n';
		}
		catch (const try_exception_wrong_exception& e)
		{
			std::cout
				<< "\n> TRY_EXCEPTION failed: " << e.function_call
				<< "\n> Threw: " << e.got
				<< "\n> Expected: " << e.expected
				<< "\n> At line: " << e.line
				<< "\n> Of file: " << e.file << '\n';
		}
		catch (...)
		{
			std::cout << "\n> Failed. Unknown error.\n";
		}
	}
	std::cout << "\nResults: " << passed << '/' << get_tests().size() << " passed.\n";
}
