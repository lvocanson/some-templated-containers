#include "../include/stc/inferred_state_machine.h"
#include <iostream>

struct Idle
{
	using exits = type_set<struct Walking>;

	static Idle enter(Walking& from)
	{
		return {};
	}
};

struct Walking
{
	Walking() = default;
	Walking(const Walking&) = delete;
	Walking(Walking&&) = default;
	Walking& operator=(const Walking&) = delete;
	Walking& operator=(Walking&&) = delete;

	using exits = type_set<Idle, struct Running>;

	static Walking enter(Idle& from)
	{
		return {};
	}

	static Walking enter(Running& from)
	{
		return {};
	}
};

struct Running
{
	using exits = type_set<Walking>;

	static Running enter(Walking& from)
	{
		return {};
	}
};

int main()
{
	using ism_t = inferred_state_machine<Running>;
	ism_t ifm;

	auto say_name = [](auto& declared_state) -> void
	{
		std::cout << typeid(declared_state).name() << std::endl;
	};

	std::cout << "To Idle: " << ifm.can_transition<Idle>() << std::endl;
	ifm.transition<Walking>();
	ifm.visit(say_name);


}
