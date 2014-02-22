#pragma once
#include "ProcessBufferHeap.h"

template<typename T>
class ScopedProcessArray
{
public:
    explicit ScopedProcessArray(int count)
        :mBuffer(allocProcessBufferOfType<T>(count))
    {

    }

    ~ScopedProcessArray()
    {
        releaseProcessBuffer(mBuffer);
    }

    T* Get() const
    {
        return mBuffer;
    }

    T& operator [](const unsigned __int64 i)
    {
        return mBuffer[i];
    }

    const T& operator [](const unsigned __int64 i) const
    {
        return mBuffer[i];
    }

private:
    T* mBuffer;
};