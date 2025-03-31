#include "../include/stc/swap_back_array.h"
#include "../include/stc/integers.h"
#include "benchmark.hpp"
#include <random>
#include <iostream>

void PrintSBA(const stc::swap_back_array<int>& sba)
{
	// Compatible with range-based for loop.
	for (int value : sba)
	{
		std::cout << value << " ";
	}
	std::cout << std::endl;
}

int main()
{
	// Initialize with a list of values, same as std::vector
	stc::swap_back_array<int32> data = {0, 1, 2, 3, 4, 5};
	PrintSBA(data);

	// Remove element at index 1 in O(1) by swapping it with the last element
	data.erase_swap(1);
	PrintSBA(data);

	// Remove 3 elements starting from index 2
	data.erase_swap(2, 3);
	PrintSBA(data);

	// Use assignment operator like std::vector
	data = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
	PrintSBA(data);

	// Delete even elements using iterators
	for (auto it = data.begin(); it != data.end();)
	{
		if (*it % 2 == 0)
		{
			it = data.erase_swap(it);
		}
		else
		{
			++it;
		}
	}
	PrintSBA(data);

	// Remove elements from index 3 to the end using iterators
	data.erase_swap(data.begin() + 3, data.end());
	PrintSBA(data);

	// Still supports std::vector methods (like resize)
	data.resize(10, -1);
	PrintSBA(data);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<fast::int16> dist;

	std::vector<fast::int16> vec_comp;
	stc::swap_back_array<fast::int16> sba_comp;

	constexpr size_t num_size = 1'000;
	fast::int16 numbers[num_size];
	for (size_t i = 0; i < num_size; ++i)
	{
		numbers[i] = dist(gen);
	}

	auto emplace_vec = [&]()
	{
		for (size_t i = 0; i < num_size; ++i)
		{
			vec_comp.emplace_back(numbers[i]);
		}
	};

	auto emplace_sba = [&]()
	{
		for (size_t i = 0; i < num_size; ++i)
		{
			sba_comp.emplace_back(numbers[i]);
		}
	};

	auto erase_vec = [&]()
	{
		for (auto it = vec_comp.begin(); it != vec_comp.end();)
		{
			if (*it % 2 == 0)
				it = vec_comp.erase(it);
			else
				++it;
		}
	};

	auto erase_sba = [&]()
	{
		for (auto it = sba_comp.begin(); it != sba_comp.end();)
		{
			if (*it % 2 == 0)
				it = sba_comp.erase_swap(it);
			else
				++it;
		}
	};

	std::cout << "\nSpeed comparison:\n\n";

	compare_benchmarks(100,
		std::make_pair<std::string_view>("Emplace vector", emplace_vec),
		std::make_pair<std::string_view>("Emplace SBA", emplace_sba)
	);

	std::cout << '\n';

	compare_benchmarks(100,
		std::make_pair<std::string_view>("Erase vector", erase_vec),
		std::make_pair<std::string_view>("Erase SBA", erase_sba));
}
