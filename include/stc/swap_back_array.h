#pragma once
#include <cassert>
#include <vector>

#include "utils/define_macros.h"
namespace stc
{

/**
 * @brief Extension of std::vector for fast O(1) removal at any index.
 *
 * This class inherits from std::vector and adds the ability to remove an element in O(1) by swapping
 * it with the last element before removal. This operation sacrifices the order of elements as a trade-off
 * for improved performance.
 *
 * @tparam T Type of elements stored in the container.
 * @tparam Allocator Allocator used for memory management (defaults to std::allocator<T>).
 */
template<typename T, typename Allocator = std::allocator<T>>
class swap_back_array : public std::vector<T, Allocator>
{
	using base = std::vector<T, Allocator>;

public:

	// Redeclare all base constructors
	using base::base;

	/**
	 * @brief Constructs a swap_back_array from an existing std::vector.
	 *
	 * @param other The std::vector to copy from.
	 */
	CONSTEXPR20 swap_back_array(const base& other) : base(other) {}

	/**
	 * @brief Constructs a swap_back_array by moving an existing std::vector.
	 *
	 * @param other The std::vector to move from.
	 */
	CONSTEXPR20 swap_back_array(base&& other) NOEXCEPT17 : base(std::move(other)) {}

	/**
	 * @brief Constructs a swap_back_array from another swap_back_array.
	 *
	 * @param other The swap_back_array to copy from.
	 */
	CONSTEXPR20 swap_back_array(const swap_back_array& other) = default;

	/**
	 * @brief Constructs a swap_back_array by moving another swap_back_array.
	 *
	 * @param other The swap_back_array to move from.
	 */
	CONSTEXPR20 swap_back_array(swap_back_array&& other) NOEXCEPT17 = default;

	/**
	 * @brief Copy assignment operator from an std::vector.
	 *
	 * @param other The std::vector to copy from.
	 * @return Reference to this swap_back_array.
	 */
	CONSTEXPR20 swap_back_array& operator=(const base& other)
	{
		base::operator=(other);
		return *this;
	}

	/**
	 * @brief Move assignment operator from an std::vector.
	 *
	 * @param other The std::vector to move from.
	 * @return Reference to this swap_back_array.
	 */
	swap_back_array& operator=(base&& other) IF_CPP17(noexcept(
		std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
		std::allocator_traits<Allocator>::is_always_equal::value))
	{
		base::operator=(std::move(other));
		return *this;
	}

	/**
	 * @brief Copy assignment operator from another swap_back_array.
	 *
	 * @param other The swap_back_array to copy from.
	 * @return Reference to this swap_back_array.
	 */
	CONSTEXPR20 swap_back_array& operator=(const swap_back_array& other) = default;

	/**
	 * @brief Move assignment operator from another swap_back_array.
	 *
	 * @param other The swap_back_array to move from.
	 * @return Reference to this swap_back_array.
	 */
	swap_back_array& operator=(swap_back_array&& other) IF_CPP17(noexcept(
		std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
		std::allocator_traits<Allocator>::is_always_equal::value)) = default;

	/**
	 * @brief Assigns the contents of an initializer list to the swap_back_array.
	 *
	 * @param ilist The initializer list to assign from.
	 * @return Reference to this swap_back_array.
	 */
	CONSTEXPR20 swap_back_array& operator=(std::initializer_list<T> ilist)
	{
		base::operator=(ilist);
		return *this;
	}

	/**
	 * @brief Removes an element at the specified index in O(1) time.
	 *
	 * This method swaps the element at the given index with the last element, then removes the last element.
	 *
	 * @note The user is responsible for providing a valid index.
	 * @note The type T must be move assignable to use this method.
	 * @note If the user is iterating over the container, the same index should be reused for the next iteration.
	 *
	 * @param element_index The index of the element to remove.
	 */
	CONSTEXPR20 void erase_swap(base::size_type element_index) noexcept(std::is_nothrow_move_assignable_v<T>)
	{
		assert(element_index < base::size());

		if (element_index + 1 != base::size())
		{
			// move element if its not already the last
			base::at(element_index) = std::move(base::back());
		}
		base::pop_back();
	}

	/**
	 * @brief Removes a range of elements starting from the specified index in O(1) time per element.
	 *
	 * This method swaps elements in the specified range with elements at the end of the container,
	 * then removes the last elements.
	 *
	 * @note The user is responsible for providing a valid range (start_index + count <= container.size()).
	 * @note The type T must be move assignable to use this method.
	 * @note If the user is iterating over the container, the same start_index should be reused for the next iteration.
	 *
	 * @param start_index The starting index of the range to remove.
	 * @param count The number of elements to remove.
	 */
	CONSTEXPR20 void erase_swap(base::size_type start_index, base::size_type count) noexcept(std::is_nothrow_move_assignable_v<T>)
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

	/**
	 * @brief Removes an element at the specified iterator in O(1) time.
	 *
	 * This method swaps the element at the given iterator with the last element, then removes the last element.
	 * The function returns a valid iterator to continue iteration safely.
	 *
	 * @note The user is responsible for providing a valid iterator (belonging to this container and not equal to end()).
	 * @note The type T must be move assignable to use this method.
	 * @note If iterating over the container, use the returned iterator to continue iteration safely.
	 *
	 * @param it The iterator pointing to the element to remove.
	 * @return it with an updated value, or end() if it was deleted.
	 */
	CONSTEXPR20 base::iterator erase_swap(base::iterator it) noexcept(std::is_nothrow_move_assignable_v<T>)
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

	/**
	 * @brief Removes the elements in range [first, last) in O(1) time per element.
	 *
	 * This method swaps the elements in the specified range with elements at the end of the container,
	 * then removes them. The function returns a valid iterator to continue iteration safely.
	 *
	 * @note The user is responsible for providing a valid range ([first, last) must be within the container).
	 * @note The type T must be move assignable to use this method.
	 * @note If iterating over the container, use the returned iterator to continue iteration safely.
	 *
	 * @param first Iterator pointing to the first element to remove.
	 * @param last Iterator pointing one past the last element to remove.
	 * @return first with an updated value, or end() if first was deleted.
	 */
	CONSTEXPR20 base::iterator erase_swap(base::iterator first, base::const_iterator last) noexcept(std::is_nothrow_move_assignable_v<T>)
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

};

} // namespace stc
#include "utils/undefine_macros.h"
