#pragma once
#ifndef DISTINCT_QUEUE_H
#define DISTINCT_QUEUE_H

#include <algorithm>
#include <deque>

template<class T, class TCompare = std::less<T>>
class distinct_queue
{
private:
    TCompare _compare;
    std::deque<T> _queue;

    struct equal_compare
    {
        const T& _rhs;
        bool operator()(const T& lhs) const
        {
            return !_compare(lhs, rhs) && !_compare(rhs, lhs);
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
        if (std::find_if(_queue.begin(), _queue.end(),
                         [&value](const T& other) {return equal_compare()(other, value); })
            == _queue.end())
        {
            _queue.push_back(value);
        }
    }
};

#endif // !DISTINCT_QUEUE_H
