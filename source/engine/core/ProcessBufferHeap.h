#pragma once

template<typename T>
inline T* allocProcessBufferOfType(int size)
{
    extern void* allocProcessBuffer(int size);
    return reinterpret_cast<T*>(allocProcessBuffer(size*sizeof(T)));
}

void releaseProcessBuffer(void* buffer);