#pragma once
#include "../include/stc/swap_back_array.h"

namespace stc
{

template<typename T, typename Allocator>
inline constexpr void swap_back_array<T, Allocator>::erase_swap(base::size_type element_index) noexcept(std::is_nothrow_move_assignable_v<T>)
{
	assert(element_index < base::size());

	if (element_index + 1 != base::size())
	{
		// move element if its not already the last
		base::at(element_index) = std::move(base::back());
	}
	base::pop_back();
}

template<typename T, typename Allocator>
inline constexpr void swap_back_array<T, Allocator>::erase_swap(base::size_type start_index, base::size_type count) noexcept(std::is_nothrow_move_assignable_v<T>)
{
	assert(start_index + count <= base::size());

	if (count == 0)
		return; // no-op

	auto erase_index = base::size() - count;
	if (start_index >= erase_index)
	{
		// no need to move, range is already at the end
		base::erase(base::end() - count, base::end());
		return;
	}

	auto next_erased_index = start_index;
	auto next_moved_index = base::size() - 1;
	do
	{
		base::at(next_erased_index) = std::move(base::at(next_moved_index));
		++next_erased_index;
		--next_moved_index;

	} while (next_erased_index < erase_index && erase_index <= next_moved_index);

	base::erase(base::end() - count, base::end());
}

template<typename T, typename Allocator>
inline constexpr swap_back_array<T, Allocator>::base::iterator swap_back_array<T, Allocator>::erase_swap(base::iterator it) noexcept(std::is_nothrow_move_assignable_v<T>)
{
	assert(base::begin() <= it && it < base::end());

	if (it + 1 != base::end())
	{
		// move element if its not already the last
		*it = std::move(base::back());
		base::pop_back();
		return it;
	}

	base::pop_back();
	return base::end();
}

template<typename T, typename Allocator>
inline constexpr swap_back_array<T, Allocator>::base::iterator swap_back_array<T, Allocator>::erase_swap(base::iterator first, base::const_iterator last) noexcept(std::is_nothrow_move_assignable_v<T>)
{
	assert(base::begin() <= first && first <= last && last <= base::end());

	if (first == last)
		return first; // no-op

	if (last == base::end())
	{
		// no need to move, range is already at the end
		base::erase(first, base::end());
		return base::end();
	}

	auto next_erased_it = first;
	auto next_moved_it = base::end() - 1;
	auto erase_it = base::end() - last + first;
	do
	{
		*next_erased_it = std::move(*next_moved_it);
		++next_erased_it;
		--next_moved_it;

	} while (next_erased_it < erase_it && erase_it <= next_moved_it);

	base::erase(erase_it, base::end());
	return first;
}

} // namespace stc
