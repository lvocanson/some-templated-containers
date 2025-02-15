#pragma once
#include <cassert>
#include <vector>

namespace stc
{

/**
 * @brief An extension of std::vector providing fast O(1) removal at any index.
 *
 * This class inherits from std::vector and adds the ability to remove an element in O(1) time by swapping
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

	// Redeclare all base constructors.
	using base::base;

	/**
	 * @brief Constructs a swap_back_array from an existing std::vector.
	 *
	 * @param other The std::vector to copy from.
	 */
	constexpr swap_back_array(const base& other) : base(other) {}

	/**
	 * @brief Constructs a swap_back_array by moving an existing std::vector.
	 *
	 * @param other The std::vector to move from.
	 */
	constexpr swap_back_array(base&& other) noexcept : base(std::move(other)) {}

	/**
	 * @brief Constructs a swap_back_array from another swap_back_array.
	 *
	 * @param other The swap_back_array to copy from.
	 */
	constexpr swap_back_array(const swap_back_array& other) = default;

	/**
	 * @brief Constructs a swap_back_array by moving another swap_back_array.
	 *
	 * @param other The swap_back_array to move from.
	 */
	constexpr swap_back_array(swap_back_array&& other) noexcept = default;

	/**
	 * @brief Copy assignment operator from an std::vector.
	 *
	 * @param other The std::vector to copy from.
	 * @return Reference to this swap_back_array.
	 */
	constexpr swap_back_array& operator=(const base& other)
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
	swap_back_array& operator=(base&& other) noexcept(
		std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
		std::allocator_traits<Allocator>::is_always_equal::value)
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
	constexpr swap_back_array& operator=(const swap_back_array& other) = default;

	/**
	 * @brief Move assignment operator from another swap_back_array.
	 *
	 * @param other The swap_back_array to move from.
	 * @return Reference to this swap_back_array.
	 */
	swap_back_array& operator=(swap_back_array&& other) noexcept(
		std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
		std::allocator_traits<Allocator>::is_always_equal::value) = default;

	/**
	 * @brief Assigns the contents of an initializer list to the swap_back_array.
	 *
	 * @param ilist The initializer list to assign from.
	 * @return Reference to this swap_back_array.
	 */
	constexpr swap_back_array& operator=(std::initializer_list<T> ilist)
	{
		base::operator=(ilist);
		return *this;
	}

	/**
	 * @brief Removes an element at the specified index in O(1) time.
	 *
	 * This method swaps the element at the given index with the last element, then removes the last element.
	 *
	 * @note The user must provide a valid index.
	 * @note The type T must be move-assignable in order to use this method.
	 * @note If the user is iterating over the container, the same index should be reused for the next iteration after each removal.
	 *
	 * @param element_index The index of the element to remove.
	 */
	constexpr void erase_swap(base::size_type element_index) noexcept(std::is_nothrow_move_assignable_v<T>);

	/**
	 * @brief Removes a range of elements starting from the specified index in O(1) time per element.
	 *
	 * This method swaps elements in the specified range with elements at the end of the container,
	 * then removes the last elements.
	 *
	 * @note The user must provide a valid range (start_index + count <= container.size()).
	 * @note The type T must be move-assignable in order to use this method.
	 *
	 * @param start_index The starting index of the range to remove.
	 * @param count The number of elements to remove.
	 */
	constexpr void erase_swap(base::size_type start_index, base::size_type count) noexcept(std::is_nothrow_move_assignable_v<T>);

	/**
	 * @brief Removes an element at the specified iterator in O(1) time.
	 *
	 * This method swaps the element at the given iterator with the last element, then removes the last element.
	 * The method returns a valid iterator, allowing safe continuation of iteration.
	 *
	 * @note The user must provide a valid iterator (belonging to this container and not equal to end()).
	 * @note The type T must be move-assignable in order to use this method.
	 * @note If iterating over the container, use the returned iterator to safely continue iteration.
	 *
	 * @param it The iterator pointing to the element to remove.
	 * @return it with an updated value, or end() if it was deleted.
	 */
	constexpr base::iterator erase_swap(base::iterator it) noexcept(std::is_nothrow_move_assignable_v<T>);

	/**
	 * @brief Removes the elements in range [first, last) in O(1) time per element.
	 *
	 * This method swaps the elements in the specified range with elements at the end of the container,
	 * then removes them. The method returns a valid iterator, allowing safe continuation of iteration.
	 *
	 * @note The user is responsible for providing a valid range ([first, last) must be within the container).
	 * @note The type T must be move-assignable in order to use this method.
	 * @note If iterating over the container, use the returned iterator to safely continue iteration.
	 *
	 * @param first Iterator pointing to the first element to remove.
	 * @param last Iterator pointing one past the last element to remove.
	 * @return first with an updated value, or end() if first was deleted.
	 */
	constexpr base::iterator erase_swap(base::iterator first, base::const_iterator last) noexcept(std::is_nothrow_move_assignable_v<T>);

};

} // namespace stc

#include "../src/swap_back_array.inl"
