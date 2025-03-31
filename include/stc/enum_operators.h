#pragma once
#include <type_traits>

/**
 * @file
 * @brief Extends the functionality of enum classes, particularly those used as flags.
 *
 * This utility enables bitwise and arithmetic operations on scoped enumerations (enum class),
 * allowing seamless interaction with their underlying type.
 */

namespace stc
{

/**
 * @brief Concept to check if a type is an enum.
 * @tparam T The type to check.
 */
template <typename T>
concept enumeration = std::is_enum_v<T>;

} // namespace stc

inline namespace stci
{

/// Arithmetic

template <stc::enumeration T> constexpr T operator|(T lhs, T rhs);
template <stc::enumeration T> constexpr T operator&(T lhs, T rhs);
template <stc::enumeration T> constexpr T operator^(T lhs, T rhs);
template <stc::enumeration T> constexpr T operator~(T value);
template <stc::enumeration T> constexpr T operator+(T lhs, std::underlying_type_t<T> rhs);
template <stc::enumeration T> constexpr T operator-(T lhs, std::underlying_type_t<T> rhs);

/// Assignment

template <stc::enumeration T> constexpr T& operator|=(T& lhs, T rhs);
template <stc::enumeration T> constexpr T& operator&=(T& lhs, T rhs);
template <stc::enumeration T> constexpr T& operator^=(T& lhs, T rhs);
template <stc::enumeration T> constexpr T& operator+=(T& lhs, std::underlying_type_t<T> rhs);
template <stc::enumeration T> constexpr T& operator-=(T& lhs, std::underlying_type_t<T> rhs);

/// Increment, decrement

template <stc::enumeration T> constexpr T& operator++(T& value);
template <stc::enumeration T> constexpr T operator++(T& value, int);
template <stc::enumeration T> constexpr T& operator--(T& value);
template <stc::enumeration T> constexpr T operator--(T& value, int);

/// Special

// Computes the difference between the values of lhs and rhs
template <stc::enumeration T> constexpr std::underlying_type_t<T> operator-(T lhs, T rhs);

// Returns true if there is at least one bit in common between lhs and rhs
template <stc::enumeration T> constexpr bool operator&&(T lhs, T rhs);

} // namespace stc

#include "../../src/enum_operators.inl"
