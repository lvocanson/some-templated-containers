#include "stc/eager_singleton.h"
#include "stc/explicit_singleton.h"
#include "stc/lazy_singleton.h"
#include "tests_framework.h"

namespace
{

std::size_t next_count()
{
	static std::size_t global_count = 0;
	return global_count++;
}

struct counted_element
{
	std::size_t count = next_count();
};

} // namespace

CREATE_TEST(eager_singleton)
{
	using singleton = stc::eager_singleton<counted_element>;

	counted_element before;
	auto& elem = singleton::instance();
	CHECK(elem.count < before.count);

	auto& elem2 = singleton::instance();
	CHECK(std::addressof(elem) == std::addressof(elem2));
}

CREATE_TEST(explicit_singleton)
{
	using singleton = stc::explicit_singleton<counted_element>;

	CHECK(not singleton::instance_constructed());

	{
		counted_element before;
		auto& elem = singleton::construct_instance();
		counted_element after;
		CHECK(singleton::instance_constructed());
		CHECK(before.count < elem.count && elem.count < after.count);

		auto& elem2 = singleton::instance();
		CHECK(std::addressof(elem) == std::addressof(elem2));
	}

	CHECK(singleton::instance_constructed());
	singleton::destruct_instance();
	CHECK(not singleton::instance_constructed());

	{
		counted_element before;
		auto& elem = singleton::construct_instance();
		counted_element after;
		CHECK(before.count < elem.count && elem.count < after.count);
		CHECK(singleton::instance_constructed());

		auto& elem2 = singleton::instance();
		CHECK(std::addressof(elem) == std::addressof(elem2));
	}
}

CREATE_TEST(lazy_singleton)
{
	using singleton = stc::lazy_singleton<counted_element>;

	counted_element before;
	auto& elem = singleton::instance();
	counted_element after;
	CHECK(before.count < elem.count && elem.count < after.count);

	auto& elem2 = singleton::instance();
	CHECK(std::addressof(elem) == std::addressof(elem2));
}
