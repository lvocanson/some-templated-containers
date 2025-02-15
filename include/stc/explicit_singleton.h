#pragma once
#include <optional>

namespace stc
{

/**
 * @brief Singleton with on-demand construction and deconstruction.
 *
 * This implementation mandates explicit construction of the singleton via construct_instance.
 * If construct_instance is called multiple times, the previous instance is deconstructed and replaced.
 * Manual deconstruction using deconstruct_instance is optional.
 *
 * @note It is recommended to use this class with the CRTP idiom to fully leverage the provided syntax.
 *
 * @tparam T The type of the singleton instance.
 */
template <typename T>
class explicit_singleton
{
public:

	/**
	 * @brief Constructs the singleton instance using the provided arguments.
	 *
	 * @note If an instance is already constructed, it is replaced by the new one.
	 *
	 * @tparam Args Parameter pack for T's constructor.
	 * @param args Arguments forwarded to T's constructor.
	 * @return T& Reference to the newly constructed singleton instance.
	 */
	template <typename... Args>
	static T& construct_instance(Args&&... args)
	{
		return instance_.emplace(std::forward<Args>(args)...);
	}

	/**
	 * @brief Checks whether the singleton instance has been constructed.
	 *
	 * @return bool True if the instance has been constructed, false otherwise.
	 */
	[[nodiscard]] static bool instance_constructed() noexcept
	{
		return instance_.has_value();
	}

	/**
	 * @brief Retrieves the singleton instance.
	 *
	 * @note Calling this function before the instance has been constructed
	 *       via construct_instance will throw an exception.
	 *
	 * @throws std::bad_optional_access if the instance has not been constructed.
	 *
	 * @return T& Reference to the singleton instance.
	 */
	[[nodiscard]] static T& instance()
	{
		return instance_.value();
	}

	/**
	 * @brief Deconstructs the singleton instance.
	 *
	 * @note Subsequent calls to instance() without re-initializing
	 *       via construct_instance will throw an exception.
	 */
	static void deconstruct_instance() noexcept
	{
		instance_.reset();
	}

protected:

	// Enables construction of T.
	explicit_singleton() = default;

private:

	// Disable copy and move semantics.
	explicit_singleton(const explicit_singleton&) = delete;
	explicit_singleton(explicit_singleton&&) = delete;
	explicit_singleton& operator=(const explicit_singleton&) = delete;
	explicit_singleton& operator=(explicit_singleton&&) = delete;

	// The optional singleton instance.
	static std::optional<T> instance_;
};

// Definition after the class, once T is fully known
template <typename T>
inline std::optional<T> explicit_singleton<T>::instance_;

} // namespace stc
