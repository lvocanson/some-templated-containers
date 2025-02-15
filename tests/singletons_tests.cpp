#include "../include/stc/eager_singleton.h"
#include "../include/stc/explicit_singleton.h"
#include "../include/stc/lazy_singleton.h"
#include "tests_framework.h"
#include <chrono>

struct timed_element
{
	std::chrono::high_resolution_clock::time_point creation_time = std::chrono::high_resolution_clock::now();
};

CREATE_TEST(eager_singleton)
{
	using singleton = stc::eager_singleton<timed_element>;

	auto now = std::chrono::high_resolution_clock::now();
	auto& elem = singleton::instance();
	CHECK(elem.creation_time < now);

	auto& elem2 = singleton::instance();
	CHECK(std::addressof(elem) == std::addressof(elem2));
}

CREATE_TEST(explicit_singleton)
{
	using singleton = stc::explicit_singleton<timed_element>;

	TRY_EXCEPTION(auto& _ = singleton::instance(), std::bad_optional_access);
	CHECK(not singleton::instance_constructed());

	{
		auto now = std::chrono::high_resolution_clock::now();
		auto& elem = singleton::construct_instance();
		CHECK(singleton::instance_constructed());
		CHECK(elem.creation_time > now);

		auto& elem2 = singleton::instance();
		CHECK(std::addressof(elem) == std::addressof(elem2));
	}

	CHECK(singleton::instance_constructed());
	singleton::deconstruct_instance();
	TRY_EXCEPTION(auto& _ = singleton::instance(), std::bad_optional_access);
	CHECK(not singleton::instance_constructed());

	{
		auto now = std::chrono::high_resolution_clock::now();
		auto& elem = singleton::construct_instance();
		CHECK(elem.creation_time > now);
		CHECK(singleton::instance_constructed());

		auto& elem2 = singleton::instance();
		CHECK(std::addressof(elem) == std::addressof(elem2));
	}
}

CREATE_TEST(lazy_singleton)
{
	using singleton = stc::lazy_singleton<timed_element>;

	auto now = std::chrono::high_resolution_clock::now();
	auto& elem = singleton::instance();
	CHECK(elem.creation_time > now);

	auto& elem2 = singleton::instance();
	CHECK(std::addressof(elem) == std::addressof(elem2));
}
