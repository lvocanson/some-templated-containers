#pragma once
#include "../include/stc/enum_operators.h"

inline namespace stci
{

template <stc::enumeration T>
inline constexpr T operator|(T lhs, T rhs)
{
	using U = std::underlying_type_t<T>;
	return T(U(lhs) | U(rhs));
}

template <stc::enumeration T>
inline constexpr T operator&(T lhs, T rhs)
{
	using U = std::underlying_type_t<T>;
	return T(U(lhs) & U(rhs));
}

template <stc::enumeration T>
inline constexpr T operator^(T lhs, T rhs)
{
	using U = std::underlying_type_t<T>;
	return T(U(lhs) ^ U(rhs));
}

template <stc::enumeration T>
inline constexpr T operator~(T value)
{
	using U = std::underlying_type_t<T>;
	return T(~U(value));
}

template <stc::enumeration T>
inline constexpr T operator+(T lhs, std::underlying_type_t<T> rhs)
{
	using U = std::underlying_type_t<T>;
	return T(U(lhs) + rhs);
}

template <stc::enumeration T>
inline constexpr T operator-(T lhs, std::underlying_type_t<T> rhs)
{
	using U = std::underlying_type_t<T>;
	return T(U(lhs) - rhs);
}

template <stc::enumeration T>
inline constexpr T& operator|=(T& lhs, T rhs)
{
	return lhs = lhs | rhs;
}

template <stc::enumeration T>
inline constexpr T& operator&=(T& lhs, T rhs)
{
	return lhs = lhs & rhs;
}

template <stc::enumeration T>
inline constexpr T& operator^=(T& lhs, T rhs)
{
	return lhs = lhs ^ rhs;
}

template <stc::enumeration T>
inline constexpr T& operator+=(T& lhs, std::underlying_type_t<T> rhs)
{
	return lhs = lhs + rhs;
}

template <stc::enumeration T>
inline constexpr T& operator-=(T& lhs, std::underlying_type_t<T> rhs)
{
	return lhs = lhs - rhs;
}

template <stc::enumeration T>
inline constexpr T& operator++(T& value)
{
	return value = value + 1;
}

template <stc::enumeration T>
inline constexpr T operator++(T& value, int)
{
	T old_v = value;
	++value;
	return old_v;
}

template <stc::enumeration T>
inline constexpr T& operator--(T& value)
{
	return value = value - 1;
}

template <stc::enumeration T>
inline constexpr T operator--(T& value, int)
{
	T old_v = value;
	--value;
	return old_v;
}

template <stc::enumeration T>
inline constexpr std::underlying_type_t<T> operator-(T lhs, T rhs)
{
	using U = std::underlying_type_t<T>;
	return U(lhs) - U(rhs);
}

template<stc::enumeration T>
inline constexpr bool operator&&(T lhs, T rhs)
{
	return (lhs & rhs) != T(0);
}

} // namespace stc
