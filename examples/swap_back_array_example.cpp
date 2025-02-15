#include "../include/stc/swap_back_array.h"
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
	stc::swap_back_array<int> data = {0, 1, 2, 3, 4, 5};
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
}
