#include "stc/enum_operators.h"
#include <gtest/gtest.h>

namespace
{

enum class test_enum : int
{
	Zero, One, Two, Three,
	Four, Five, Max = ~int()
};

} // namespace

TEST(enum_operators, arithmetic)
{
	using enum test_enum;

	EXPECT_EQ((Zero | One), One);
	EXPECT_EQ((Three & Two), Two);
	EXPECT_EQ((Four ^ Five), One);
	EXPECT_EQ(~Zero, Max);
	EXPECT_EQ(Two + 2, Four);
	EXPECT_EQ(Five - 2, Three);
}

TEST(enum_operators, assignment)
{
	using enum test_enum;

	test_enum a = One;
	a |= Two;
	EXPECT_EQ(a, Three);

	a &= Two;
	EXPECT_EQ(a, Two);

	a ^= Two;
	EXPECT_EQ(a, Zero);

	a += 3;
	EXPECT_EQ(a, Three);

	a -= 1;
	EXPECT_EQ(a, Two);
}

TEST(enum_operators, increment_decrement)
{
	using enum test_enum;

	test_enum a = Two;
	EXPECT_EQ(++a, Three);
	EXPECT_EQ(a++, Three);
	EXPECT_EQ(a, Four);
	EXPECT_EQ(--a, Three);
	EXPECT_EQ(a--, Three);
	EXPECT_EQ(a, Two);
}

TEST(enum_operators, special)
{
	using enum test_enum;

	EXPECT_EQ((Five - Two), 3);
	EXPECT_TRUE(Three && Two);
	EXPECT_FALSE(Four && Zero);
}
