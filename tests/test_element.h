#pragma once
#include <iostream>

struct test_element_data
{
	size_t ctor_counter = 0;
	size_t copy_counter = 0;
	size_t move_counter = 0;
	size_t dtor_counter = 0;
};

std::ostream& operator<<(std::ostream& os, const test_element_data& data)
{
	os << "Ctor Counter : " << data.ctor_counter
		<< ", Copy Counter: " << data.copy_counter
		<< ", Move Counter: " << data.move_counter
		<< ", Destroy Counter: " << data.dtor_counter;
	return os;
}

struct test_element
{
	test_element() = delete;

	test_element(size_t id, test_element_data& data)
		: id(id), data(data)
	{
		++data.ctor_counter;
	}

	~test_element()
	{
		++data.dtor_counter;
	}

	test_element(const test_element& other)
		: id(other.id), data(other.data)
	{
		++data.copy_counter;
	}

	test_element& operator=(const test_element& other)
	{
		if (this != &other)
		{
			id = other.id;
			++data.copy_counter;
		}
		return *this;
	}

	test_element(test_element&& other) noexcept
		: id(other.id), data(other.data)
	{
		++data.move_counter;
	}

	test_element& operator=(test_element&& other) noexcept
	{
		if (this != &other)
		{
			id = other.id;
			++data.move_counter;
		}
		return *this;
	}

	size_t id;
	test_element_data& data;
};
