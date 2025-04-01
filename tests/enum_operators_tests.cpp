#include "../include/stc/enum_operators.h"
#include "tests_framework.h"

enum class test_enum : int
{
	Zero, One, Two, Three,
	Four, Five, Max = ~int()
};

CREATE_TEST(enum_operators__arithmetic)
{
	using enum test_enum;

	CHECK((Zero | One) == One);
	CHECK((Three & Two) == Two);
	CHECK((Four ^ Five) == One);
	CHECK(~Zero == Max);
	CHECK(Two + 2 == Four);
	CHECK(Five - 2 == Three);
}

CREATE_TEST(enum_operators__assignment)
{
	using enum test_enum;

	test_enum a = One;
	a |= Two;
	CHECK(a == Three);

	a &= Two;
	CHECK(a == Two);

	a ^= Two;
	CHECK(a == Zero);

	a += 3;
	CHECK(a == Three);

	a -= 1;
	CHECK(a == Two);
}

CREATE_TEST(enum_operators__increment_decrement)
{
	using enum test_enum;

	test_enum a = Two;
	CHECK(++a == Three);
	CHECK(a++ == Three);
	CHECK(a == Four);
	CHECK(--a == Three);
	CHECK(a-- == Three);
	CHECK(a == Two);
}

CREATE_TEST(enum_operators__special)
{
	using enum test_enum;

	CHECK((Five - Two) == 3);
	CHECK(Three && Two);
	CHECK(not (Four && Zero));
}
