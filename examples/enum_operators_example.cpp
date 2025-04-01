#include "../include/stc/enum_operators.h"
#include <iostream>

enum class SomeFlag : uint8_t
{
	OptionA = 1 << 0,
	OptionB = 1 << 1,
	OptionC = 1 << 2,
	OptionD = 1 << 3,
	OptionE = 1 << 4,
};

int main()
{
	// Combine flags using bitwise OR.
	SomeFlag bcd = SomeFlag::OptionB | SomeFlag::OptionC | SomeFlag::OptionD;
	std::cout << "Initial flags set: OptionB, OptionC, OptionD.\n";

	// Check if OptionA is present using bitwise AND.
	if ((bcd & SomeFlag::OptionA) == SomeFlag::OptionA)
	{
		std::cout << "OptionA is set.\n";
	}

	// Check if OptionB is present using bitwise AND.
	if ((bcd & SomeFlag::OptionB) == SomeFlag::OptionB)
	{
		std::cout << "OptionB is set.\n";
	}

	// Check if OptionD OR OptionE is present using special &&.
	if (bcd && (SomeFlag::OptionD | SomeFlag::OptionE))
	{
		std::cout << "OptionD or OptionE is set.\n";
	}

	// Remove OptionB.
	bcd &= ~SomeFlag::OptionB;
	std::cout << "Removed OptionB.\n";
	if ((bcd & SomeFlag::OptionB) == SomeFlag::OptionB)
	{
		std::cout << "OptionB is set (unexpected!).\n";
	}
	else
	{
		std::cout << "OptionB is not set.\n";
	}

	// Add OptionA.
	bcd |= SomeFlag::OptionA;
	std::cout << "Added OptionA.\n";
	if ((bcd & SomeFlag::OptionA) == SomeFlag::OptionA)
	{
		std::cout << "OptionA is set.\n";
	}
}
