#include "stc/swap_back_array.h"
#include "test_element.h"
#include <gtest/gtest.h>

namespace
{

stc::swap_back_array<test_element> test_sba(size_t count, test_element_data& data)
{
	stc::swap_back_array<test_element> sba;
	sba.reserve(count);
	for (size_t i = 0; i < count; ++i)
	{
		sba.emplace_back(i, data);
	}
	return sba;
}

bool find_test_element_by_id(const stc::swap_back_array<test_element>& sba, size_t id)
{
	for (auto& te : sba)
	{
		if (te.id == id)
		{
			return true;
		}
	}
	return false;
}

} // namespace

TEST(swap_back_array, erase_index)
{
	test_element_data data;
	auto sba = test_sba(10, data);
	size_t erased_id;

	erased_id = sba[2].id;
	sba.erase_swap(2);

	EXPECT_EQ(sba.size(), 9);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 1);
	EXPECT_EQ(data.move_counter, 1);

	erased_id = sba[5].id;
	sba.erase_swap(5);

	EXPECT_EQ(sba.size(), 8);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 2);
	EXPECT_EQ(data.move_counter, 2);

	erased_id = sba[0].id;
	sba.erase_swap(0);

	EXPECT_EQ(sba.size(), 7);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 3);
	EXPECT_EQ(data.move_counter, 3);
}

TEST(swap_back_array, erase_index_at_end)
{
	test_element_data data;
	auto sba = test_sba(10, data);
	size_t erased_id;

	erased_id = sba[sba.size() - 1].id;
	sba.erase_swap(sba.size() - 1);

	EXPECT_EQ(sba.size(), 9);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 1);
	EXPECT_EQ(data.move_counter, 0);

	erased_id = sba[sba.size() - 1].id;
	sba.erase_swap(sba.size() - 1);

	EXPECT_EQ(sba.size(), 8);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 2);
	EXPECT_EQ(data.move_counter, 0);

	erased_id = sba[sba.size() - 1].id;
	sba.erase_swap(sba.size() - 1);

	EXPECT_EQ(sba.size(), 7);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 3);
	EXPECT_EQ(data.move_counter, 0);
}

TEST(swap_back_array, erase_index_range)
{
	test_element_data data;
	auto sba = test_sba(30, data);
	stc::swap_back_array<size_t> erased_ids;

	size_t index = 1;
	size_t count = 1;
	for (size_t i = index; i < index + count; ++i)
		erased_ids.push_back(sba[i].id);

	sba.erase_swap(index, count);

	EXPECT_EQ(sba.size(), 29);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 1);
	EXPECT_EQ(data.move_counter, 1);

	index = 2;
	count = 4;
	for (size_t i = index; i < index + count; ++i)
		erased_ids.push_back(sba[i].id);

	sba.erase_swap(index, count);

	EXPECT_EQ(sba.size(), 25);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 5);
	EXPECT_EQ(data.move_counter, 5);

	index = 3;
	count = 10;
	for (size_t i = index; i < index + count; ++i)
		erased_ids.push_back(sba[i].id);

	sba.erase_swap(index, count);

	EXPECT_EQ(sba.size(), 15);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 15);
	EXPECT_EQ(data.move_counter, 15);
}

TEST(swap_back_array, erase_index_range_near_end)
{
	test_element_data data;
	auto sba = test_sba(30, data);
	stc::swap_back_array<size_t> erased_ids;

	size_t index = 28;
	size_t count = 1;
	for (size_t i = index; i < index + count; ++i)
		erased_ids.push_back(sba[i].id);

	sba.erase_swap(index, count);

	EXPECT_EQ(sba.size(), 29);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 1);
	EXPECT_EQ(data.move_counter, 1);

	index = 24;
	count = 4;
	for (size_t i = index; i < index + count; ++i)
		erased_ids.push_back(sba[i].id);

	sba.erase_swap(index, count);

	EXPECT_EQ(sba.size(), 25);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 5);
	EXPECT_EQ(data.move_counter, 2);

	index = 14;
	count = 10;
	for (size_t i = index; i < index + count; ++i)
		erased_ids.push_back(sba[i].id);

	sba.erase_swap(index, count);

	EXPECT_EQ(sba.size(), 15);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 15);
	EXPECT_EQ(data.move_counter, 3);
}

TEST(swap_back_array, erase_index_range_at_end)
{
	test_element_data data;
	auto sba = test_sba(30, data);
	stc::swap_back_array<size_t> erased_ids;

	size_t index = 29;
	size_t count = 1;
	for (size_t i = index; i < index + count; ++i)
		erased_ids.push_back(sba[i].id);

	sba.erase_swap(index, count);

	EXPECT_EQ(sba.size(), 29);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 1);
	EXPECT_EQ(data.move_counter, 0);

	index = 25;
	count = 4;
	for (size_t i = index; i < index + count; ++i)
		erased_ids.push_back(sba[i].id);

	sba.erase_swap(index, count);

	EXPECT_EQ(sba.size(), 25);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 5);
	EXPECT_EQ(data.move_counter, 0);

	index = 15;
	count = 10;
	for (size_t i = index; i < index + count; ++i)
		erased_ids.push_back(sba[i].id);

	sba.erase_swap(index, count);

	EXPECT_EQ(sba.size(), 15);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 15);
	EXPECT_EQ(data.move_counter, 0);
}

TEST(swap_back_array, erase_iterator)
{
	test_element_data data;
	auto sba = test_sba(10, data);
	auto it = sba.begin();
	size_t erased_id;

	std::advance(it, 1);
	erased_id = it->id;
	sba.erase_swap(it);

	EXPECT_EQ(sba.size(), 9);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 1);
	EXPECT_EQ(data.move_counter, 1);

	std::advance(it, 2);
	erased_id = it->id;
	sba.erase_swap(it);

	EXPECT_EQ(sba.size(), 8);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 2);
	EXPECT_EQ(data.move_counter, 2);

	std::advance(it, 3);
	erased_id = it->id;
	sba.erase_swap(it);

	EXPECT_EQ(sba.size(), 7);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 3);
	EXPECT_EQ(data.move_counter, 3);
}

TEST(swap_back_array, erase_iterator_at_end)
{
	test_element_data data;
	auto sba = test_sba(10, data);
	size_t erased_id;

	auto it = --sba.end();
	erased_id = it->id;
	sba.erase_swap(it);

	EXPECT_EQ(sba.size(), 9);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 1);
	EXPECT_EQ(data.move_counter, 0);

	it = --sba.end();
	erased_id = it->id;
	sba.erase_swap(it);

	EXPECT_EQ(sba.size(), 8);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 2);
	EXPECT_EQ(data.move_counter, 0);

	it = --sba.end();
	erased_id = it->id;
	sba.erase_swap(it);

	EXPECT_EQ(sba.size(), 7);
	EXPECT_FALSE(find_test_element_by_id(sba, erased_id));
	EXPECT_EQ(data.ctor_counter, 10);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 3);
	EXPECT_EQ(data.move_counter, 0);
}

TEST(swap_back_array, erase_iterator_range)
{
	test_element_data data;
	auto sba = test_sba(30, data);
	stc::swap_back_array<size_t> erased_ids;

	auto first = sba.begin();
	auto last = sba.begin() + 1;
	for (auto it = first; it != last; ++it)
		erased_ids.push_back(it->id);

	sba.erase_swap(first, last);

	EXPECT_EQ(sba.size(), 29);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 1);
	EXPECT_EQ(data.move_counter, 1);

	first = sba.begin() + 2;
	last = sba.begin() + 6;
	for (auto it = first; it != last; ++it)
		erased_ids.push_back(it->id);

	sba.erase_swap(first, last);

	EXPECT_EQ(sba.size(), 25);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 5);
	EXPECT_EQ(data.move_counter, 5);

	first = sba.begin() + 3;
	last = sba.begin() + 13;
	for (auto it = first; it != last; ++it)
		erased_ids.push_back(it->id);

	sba.erase_swap(first, last);

	EXPECT_EQ(sba.size(), 15);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 15);
	EXPECT_EQ(data.move_counter, 15);
}

TEST(swap_back_array, erase_iterator_range_near_end)
{
	test_element_data data;
	auto sba = test_sba(30, data);
	stc::swap_back_array<size_t> erased_ids;

	auto first = sba.end() - 2;
	auto last = sba.end() - 1;
	for (auto it = first; it != last; ++it)
		erased_ids.push_back(it->id);

	sba.erase_swap(first, last);

	EXPECT_EQ(sba.size(), 29);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 1);
	EXPECT_EQ(data.move_counter, 1);

	first = sba.end() - 5;
	last = sba.end() - 1;
	for (auto it = first; it != last; ++it)
		erased_ids.push_back(it->id);

	sba.erase_swap(first, last);

	EXPECT_EQ(sba.size(), 25);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 5);
	EXPECT_EQ(data.move_counter, 2);

	first = sba.end() - 11;
	last = sba.end() - 1;
	for (auto it = first; it != last; ++it)
		erased_ids.push_back(it->id);

	sba.erase_swap(first, last);

	EXPECT_EQ(sba.size(), 15);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 15);
	EXPECT_EQ(data.move_counter, 3);
}

TEST(swap_back_array, erase_iterator_range_at_end)
{
	test_element_data data;
	auto sba = test_sba(30, data);
	stc::swap_back_array<size_t> erased_ids;

	auto first = sba.end() - 1;
	auto last = sba.end();
	for (auto it = first; it != last; ++it)
		erased_ids.push_back(it->id);

	sba.erase_swap(first, last);

	EXPECT_EQ(sba.size(), 29);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 1);
	EXPECT_EQ(data.move_counter, 0);

	first = sba.end() - 4;
	last = sba.end();
	for (auto it = first; it != last; ++it)
		erased_ids.push_back(it->id);

	sba.erase_swap(first, last);

	EXPECT_EQ(sba.size(), 25);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 5);
	EXPECT_EQ(data.move_counter, 0);

	first = sba.end() - 10;
	last = sba.end();
	for (auto it = first; it != last; ++it)
		erased_ids.push_back(it->id);

	sba.erase_swap(first, last);

	EXPECT_EQ(sba.size(), 15);
	for (size_t id : erased_ids)
	{
		EXPECT_FALSE(find_test_element_by_id(sba, id));
	}
	EXPECT_EQ(data.ctor_counter, 30);
	EXPECT_EQ(data.copy_counter, 0);
	EXPECT_EQ(data.dtor_counter, 15);
	EXPECT_EQ(data.move_counter, 0);
}
