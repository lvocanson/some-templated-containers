#pragma once
#include <cassert>
#include <type_traits>

namespace stc
{

/**
 * @brief Singleton with on-demand construction and destruction.
 *
 * This implementation mandates explicit construction of the singleton via construct_instance.
 * If construct_instance is called multiple times, the previous instance is destructed and replaced.
 * Manual destruction using destruct_instance is optional.
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
		if (constructed_)
			storage_.instance_.~T();
		new (&storage_.instance_) T(std::forward<Args>(args)...);
		constructed_ = true;
		return storage_.instance_;
	}

	/**
	 * @brief Checks whether the singleton instance has been constructed.
	 *
	 * @return bool True if the instance has been constructed, false otherwise.
	 */
	[[nodiscard]] static bool instance_constructed() noexcept
	{
		return constructed_;
	}

	/**
	 * @brief Retrieves the singleton instance.
	 *
	 * @note Calling this function before the instance has been constructed is undefined behavior.
	 *
	 * @return T& Reference to the singleton instance.
	 */
	[[nodiscard]] static T& instance()
	{
		assert(constructed_ && "Accessing uninitialized singleton instance.");
		return storage_.instance_;
	}

	/**
	 * @brief Destructs the singleton instance.
	 *
	 * @note Subsequent calls to instance() without re-initializing is undefined behavior.
	 */
	static void destruct_instance() noexcept
	{
		if (constructed_)
		{
			storage_.instance_.~T();
			constructed_ = false;
		}
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

	union storage
	{
		storage() { /* Leave instance_ uninitialized. */ };
		~storage() { destruct_instance(); }

		T instance_;
	};
	
	static storage storage_;
	static inline bool constructed_ = false;
};

// Definition after the class, once T is fully known.
template <typename T>
inline explicit_singleton<T>::storage explicit_singleton<T>::storage_;

} // namespace stc
