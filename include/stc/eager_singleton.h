#pragma once

namespace stc
{

/**
 * @brief Singleton instantiated during static initialization.
 *
 * This singleton instance is created as part of static initialization, ensuring its availability
 * from the start of the program. However, it is subject to the 'Static Initialization Order Fiasco'.
 * @see https://en.cppreference.com/w/cpp/language/siof
 *
 * @note It is recommended to use this class with the CRTP idiom to fully leverage the provided syntax.
 *
 * @tparam T The type of the singleton instance.
 */
template <typename T>
class eager_singleton
{
public:

	/**
	 * @brief Retrieves the singleton instance.
	 *
	 * @return T& Reference to the singleton instance.
	 */
	[[nodiscard]] static T& instance()
	{
		return instance_;
	}

protected:

	// Enables construction of T.
	eager_singleton() = default;

private:

	// Disable copy and move semantics.
	eager_singleton(const eager_singleton&) = delete;
	eager_singleton(eager_singleton&&) = delete;
	eager_singleton& operator=(const eager_singleton&) = delete;
	eager_singleton& operator=(eager_singleton&&) = delete;

	// The eagerly instantiated singleton instance.
	static T instance_;
};

// Definition after the class, once T is fully known
template <typename T>
T eager_singleton<T>::instance_;

} // namespace stc
