#pragma once

#include <limits>

#include <task-dispatcher/common/panic.hh>

namespace td::container
{
// This is not synchronized at all, TODO: Replace with Yukov or similar
template <class T, size_t N>
class FIFOQueue
{
    static_assert (N > 0);
    static_assert(N < size_t(std::numeric_limits<int>::max()), "FIFOQueue too large");

private:
    int mHead = -1;
    int mTail = -1;
    T mData[N];

public:
    FIFOQueue() = default;

    bool dequeue(T& out_ref)
    {
        if (empty())
            return false;

        out_ref = mData[mTail];

        if (mTail == mHead)
        {
            // Queue now empty
            mTail = -1;
            mHead = -1;
        }
        else
        {
            // Increment tail
            ++mTail;
            if (mTail >= N)
                mTail -= N;
        }

        return true;
    }

    void enqueue(T const& val)
    {
        TD_PANIC_IF(full(), "FIFOQueue Full");

        if (empty())
            mTail = 0;

        // Increment head
        ++mHead;
        if (mHead >= N)
            mHead -= N;

        mData[mHead] = val;
    }

    bool empty() const { return mTail == -1; }
    bool full() const
    {
        auto next_head = mHead + 1;
        if (next_head >= N)
            next_head -= N;

        return next_head == mTail;
    }

    FIFOQueue(FIFOQueue const& other) = delete;
    FIFOQueue(FIFOQueue&& other) noexcept = delete;
    FIFOQueue& operator=(FIFOQueue const& other) = delete;
    FIFOQueue& operator=(FIFOQueue&& other) noexcept = delete;
};

}
