#pragma once

#include <vector>

template <class T>
class ring
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_pointer = const T *;

private:
    class sentinel;

    class iterator
    {
    public:
        using iterator_concept = forward_iterator_tag;
        using value_type = T;
        using differece_type = ptrdiff_t;
        using pointer = T *;
        using reference = T &;

    private:
        std::size_t pos;
        std::size_t capacity;

    public:
        iterator()
        {
        }

        bool operator!=(const sentinel &sent) const
        {
            return true;
        }
    };

    class sentinel
    {
        bool operator!=(const iterator &itr) const
        {
            return true;
        }
    };

    std::vector<T> buffer_;
    iterator itr_head_;
    iterator itr_tail_;

public:
    ring(const std::size_t capacity)
    {
        buffer_.reserve(capacity + 1);
    }

    constexpr iterator begin() const
    {
        return m_itr_;
    }

    constexpr sentinel end() const
    {
        return sentinel{itr_tail_};
    }
};
