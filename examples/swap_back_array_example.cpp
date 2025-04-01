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


	std::cout << "\nSpeed comparison:\n\n";

	std::vector<size_t> vec_comp;
	stc::swap_back_array<size_t> sba_comp;

	auto emplace_vec = [&](size_t i)
	{
		vec_comp.emplace_back(i);
	};

	auto emplace_sba = [&](size_t i)
	{
		sba_comp.emplace_back(i);
	};

	auto erase_vec = [&](size_t i)
	{
		for (auto it = vec_comp.begin(); it != vec_comp.end();)
		{
			if (*it == i)
				it = vec_comp.erase(it);
			else
				++it;
		}
	};

	auto erase_sba = [&](size_t i)
	{
		for (auto it = sba_comp.begin(); it != sba_comp.end();)
		{
			if (*it == i)
				it = sba_comp.erase_swap(it);
			else
				++it;
		}
	};

	// The order of execution matters. SBA's emplace is inherited
	// from vector, the speed should be the same.
	benchmark(100'000)
		.add("Emplace SBA", emplace_sba)
		.add("Emplace vector", emplace_vec)
		.add("Emplace SBA 2", emplace_sba)
		.add("Emplace vector 2", emplace_vec)
		.print_results();

	std::cout << '\n';

	// two elements to remove per iteration
	benchmark(100'000)
		.add("Erase SBA", erase_sba)
		.add("Erase vector", erase_vec)
		.print_results();
}
