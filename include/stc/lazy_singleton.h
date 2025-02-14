#pragma once

namespace stc
{

/**
 * @brief Singleton created upon the first access to its instance.
 *
 * @note It is recommended to use this class with the CRTP idiom to fully leverage the provided syntax.
 *
 * @tparam T The type of the singleton instance.
 */
template <typename T>
class lazy_singleton
{
public:

	/**
	 * @brief Retrieves the singleton instance.
	 *
	 * On the first call, constructs and returns a reference to the instance.
	 * On subsequent calls, returns a reference to the already constructed instance.
	 *
	 * @return T& Reference to the singleton instance.
	 */
	[[nodiscard]] static T& instance()
	{
		static T instance;
		return instance;
	}

protected:

	// Enables construction of T.
	lazy_singleton() = default;

private:

	// Disable copy and move semantics.
	lazy_singleton(const lazy_singleton&) = delete;
	lazy_singleton(lazy_singleton&&) = delete;
	lazy_singleton& operator=(const lazy_singleton&) = delete;
	lazy_singleton& operator=(lazy_singleton&&) = delete;
};

} // namespace stc
