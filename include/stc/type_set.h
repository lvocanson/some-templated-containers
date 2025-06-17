#pragma once
#include "concepts_and_traits.h"

/**
 * @brief A compile-time set of types with enforced uniqueness.
 *
 * Provides operations to add, remove, merge, or apply templates to the set of types.
 * Requires all contained types to be unique.
 *
 * @tparam Ts Unique types in the set.
 */
template<typename... Ts> requires unique_types<Ts...>
struct type_set
{
private: // Internal forward declarations
	template <typename...> struct add_impl;
	template <typename...> struct remove_impl;

public: /* === Public Interface === */

	/**
	 * @brief Returns a new type_set including all types in this set and As.
	 *        Duplicates in As are ignored.
	 * @tparam As Types to add.
	 */
	template <typename... As>
	using add = add_impl<As...>::type;

	/**
	 * @brief Returns a new type_set with all types from Rs removed, if present.
	 * @tparam Rs Types to remove.
	 */
	template <typename... Rs>
	using remove = remove_impl<Rs...>::type;

	/**
	 * @brief Returns a new type_set with all types from this and another set.
	 * @tparam Other Another type_set.
	 */
	template <template_instance_of<::type_set> Other>
	using combine = Other::template add<Ts...>;

	/**
	 * @brief Applies the current type list to a variadic template.
	 * @tparam Template A class template taking Ts... as parameters.
	 * @tparam Ps Prefix types to append before the current list.
	 */
	template <template <typename...> typename Template, typename... Ps>
	using apply = Template<Ps..., Ts...>;

	/// Number of types in the set.
	static constexpr size_t size = sizeof...(Ts);

	/// Whether the set is empty.
	static constexpr bool empty = size == size_t(0);

	template <typename T>
	static constexpr bool contains = contained_in<T, Ts...>;

	template <typename... Us>
	static constexpr bool contains_all = (contains<Us> && ...);

	template <typename... Us>
	static constexpr bool contains_any = (contains<Us> || ...);

	/**
	 * @brief Checks whether another type_set contains exactly the same types (regardless of order).
	 * @tparam Other Another type_set.
	 */
	template <template_instance_of<::type_set> Other>
	static constexpr bool equals = size == Other::size && Other::template contains_all<Ts...>;


private: // Internal implementations

	// Add: Skip if already contained
	template <contained_in<Ts...> A, typename... As>
	struct add_impl<A, As...>
	{
		using type = add_impl<As...>::type;
	};

	// Add: Append if not already in set
	template <typename A, typename... As>
	struct add_impl<A, As...>
	{
		using type = type_set<Ts..., A>::template add<As...>;
	};

	// Add: End recursion
	template <>
	struct add_impl<>
	{
		using type = type_set;
	};

	// Remove: If R is in set, remove it
	template <contained_in<Ts...> R, typename... Rs>
	struct remove_impl<R, Rs...>
	{
		template <typename... Us> struct sub_set;

		// Match found, remove it
		template <std::same_as<R> U, typename... Us>
		struct sub_set<U, Us...>
		{
			using type = type_set<Us...>::template remove<R, Rs...>;
		};

		// Cycle U to the end if not R
		template <typename U, typename... Us>
		struct sub_set<U, Us...>
		{
			using type = sub_set<Us..., U>::type;
		};

		using type = sub_set<Ts...>::type;
	};

	// Remove: Skip if not found
	template <typename R, typename... Rs>
	struct remove_impl<R, Rs...>
	{
		using type = remove_impl<Rs...>::type;
	};

	// Remove: End recursion
	template <>
	struct remove_impl<>
	{
		using type = type_set;
	};
};
