#pragma once
#include <concepts>
#include <type_traits>

/**
 * @brief Checks whether a type T is contained in a list of types Ts.
 * @tparam T The type to check.
 * @tparam Ts The list of types in which T may be contained.
 */
template<typename T, typename... Ts>
concept contained_in = (std::same_as<T, Ts> || ...);

/**
 * @brief Trait to detect whether a type is an instantiation of a given template.
 * @tparam T The type to check.
 * @tparam Template The template to test against.
 */
template <typename T, template <typename...> typename Template>
struct is_template_instance_of : std::false_type {};

/**
 * @brief Specialization of is_template_instance_of for matching template instantiations.
 */
template <template <typename...> typename Template, typename... Args>
struct is_template_instance_of<Template<Args...>, Template> : std::true_type {};

/**
 * @brief Concept that checks if a type is an instantiation of a specific template.
 * @tparam T The type to test.
 * @tparam Template The template to match.
 */
template <typename T, template <typename...> typename Template>
concept template_instance_of = is_template_instance_of<T, Template>::value;

/**
 * @brief Trait that checks if a list of types are all unique.
 * @tparam Ts The types to check.
 */
template<typename...>
struct are_unique_types : std::true_type {};

/**
 * @brief Recursive implementation that checks for uniqueness of types.
 */
template<typename T, typename... Rest>
struct are_unique_types<T, Rest...> : std::bool_constant<(!contained_in<T, Rest...>&& are_unique_types<Rest...>::value)> {};

/**
 * @brief Concept that requires all types in a parameter pack to be unique.
 * @tparam Ts The types to verify.
 */
template<typename... Ts>
concept unique_types = are_unique_types<Ts...>::value;
