#include "stc/eager_singleton.h"
#include "stc/explicit_singleton.h"
#include "stc/lazy_singleton.h"
#include <gtest/gtest.h>

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

TEST(singletons, eager_singleton)
{
	using singleton = stc::eager_singleton<counted_element>;

	counted_element before;
	auto& elem = singleton::instance();
	EXPECT_LT(elem.count, before.count);

	auto& elem2 = singleton::instance();
	EXPECT_EQ(std::addressof(elem), std::addressof(elem2));
}

TEST(singletons, explicit_singleton)
{
	using singleton = stc::explicit_singleton<counted_element>;

	EXPECT_FALSE(singleton::instance_constructed());

	{
		counted_element before;
		auto& elem = singleton::construct_instance();
		counted_element after;
		EXPECT_TRUE(singleton::instance_constructed());
		EXPECT_TRUE(before.count < elem.count && elem.count < after.count);

		auto& elem2 = singleton::instance();
		EXPECT_EQ(std::addressof(elem), std::addressof(elem2));
	}

	EXPECT_TRUE(singleton::instance_constructed());
	singleton::destruct_instance();
	EXPECT_FALSE(singleton::instance_constructed());

	{
		counted_element before;
		auto& elem = singleton::construct_instance();
		counted_element after;
		EXPECT_TRUE(before.count < elem.count && elem.count < after.count);
		EXPECT_TRUE(singleton::instance_constructed());

		auto& elem2 = singleton::instance();
		EXPECT_EQ(std::addressof(elem), std::addressof(elem2));
	}
}

TEST(singletons, lazy_singleton)
{
	using singleton = stc::lazy_singleton<counted_element>;

	counted_element before;
	auto& elem = singleton::instance();
	counted_element after;
	EXPECT_TRUE(before.count < elem.count && elem.count < after.count);

	auto& elem2 = singleton::instance();
	EXPECT_EQ(std::addressof(elem), std::addressof(elem2));
}
