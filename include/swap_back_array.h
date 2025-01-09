#pragma once
#include <concepts>
#include <vector>

namespace stc
{

/**
 * @brief A wrapper around std::vector that provides fast removal of elements at any index
 *        at the cost of maintaining element order.
 *
 * This class is designed for scenarios where frequent and efficient removals are required.
 * It swaps the element to be removed with the last element, then removes the last element,
 * ensuring O(1) complexity for deletions.
 */
template<typename T, typename A = std::vector<T>::allocator_type>
class swap_back_array
{
public:

    // underlying container type
    using uct = std::vector<T, A>;

    template<typename ...params> requires std::constructible_from<uct, params...>
    swap_back_array(params&& ...parameters)
        : m_data(std::forward<params>(parameters)...)
    {
    }

    void reserve(uct::size_type capacity)
    {
        m_data.reserve(capacity);
    }

    template<typename ...params>
    void emplace_back(params&& ...parameters)
    {
        m_data.emplace_back(std::forward<params>(parameters)...);
    }

    void erase(uct::size_type element_index)
    {
        m_data[element_index] = std::move(m_data.back());
        m_data.pop_back();
    }

    [[nodiscard]] uct::size_type size() const
    {
        return m_data.size();
    }

    [[nodiscard]] uct::size_type capacity() const
    {
        return m_data.capacity();
    }

    [[nodiscard]] uct::reference operator[](uct::size_type element_index)
    {
        return m_data[element_index];
    }

    [[nodiscard]] uct::const_reference operator[](uct::size_type element_index) const
    {
        return m_data[element_index];
    }

    [[nodiscard]] uct::iterator begin()
    {
        return m_data.begin();
    }

    [[nodiscard]] uct::const_iterator begin() const
    {
        return m_data.begin();
    }

    [[nodiscard]] uct::iterator end()
    {
        return m_data.end();
    }

    [[nodiscard]] uct::const_iterator end() const
    {
        return m_data.end();
    }

    [[nodiscard]] uct& get_underlying_container()
    {
        return m_data;
    }

    [[nodiscard]] const uct& get_underlying_container() const
    {
        return m_data;
    }

protected:
    uct m_data;
};

} // namespace stc
