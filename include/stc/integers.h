#pragma once
#include <cstdint>

inline namespace stc
{

/**
 * @brief Namespace for integer type aliases with minimum guaranteed width.
 */
inline namespace least
{

using int8 = std::int_least8_t;
using int16 = std::int_least16_t;
using int32 = std::int_least32_t;
using int64 = std::int_least64_t;

using uint8 = std::uint_least8_t;
using uint16 = std::uint_least16_t;
using uint32 = std::uint_least32_t;
using uint64 = std::uint_least64_t;

} // namespace least

/**
 * @brief Namespace for integer type aliases optimized for speed.
 */
namespace fast
{

using int8 = std::int_fast8_t;
using int16 = std::int_fast16_t;
using int32 = std::int_fast32_t;
using int64 = std::int_fast64_t;

using uint8 = std::uint_fast8_t;
using uint16 = std::uint_fast16_t;
using uint32 = std::uint_fast32_t;
using uint64 = std::uint_fast64_t;

} // namespace fast

} // namespace stc
