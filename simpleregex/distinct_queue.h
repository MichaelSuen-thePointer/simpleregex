#pragma once
#ifndef DISTINCT_QUEUE_H
#define DISTINCT_QUEUE_H

#include <algorithm>
#include <deque>

namespace pl
{
namespace container
{

template<class T, class TEqual = std::equal_to<T>>
class distinct_queue
{
private:
    std::deque<T> _queue;

    struct equal_compare
    {
        const T& _rhs;
        bool operator()(const T& lhs) const
        {
            return TEqual()(lhs, _rhs);
        }
        equal_compare(const T& rhs)
            :_rhs(rhs)
        {
        }
    };
public:
    T& front()
    {
        return _queue.front();
    }
    T& back()
    {
        return _queue.back();
    }
    void push(const T& value)
    {
        if (std::find_if(_queue.begin(), _queue.end(), equal_compare(value))
            == _queue.end())
        {
            _queue.push_back(value);
        }
    }
    void pop()
    {
        _queue.pop_front();
    }
    size_t size()
    {
        return _queue.size();
    }
};

}
}
#endif // !DISTINCT_QUEUE_H
