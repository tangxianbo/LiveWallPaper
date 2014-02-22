#include "ProcessBufferHeap.h"
#include <stdlib.h>

namespace
{
    class ProcessBufferHeap
    {
    public:
        ProcessBufferHeap(int bufferSize);
        ~ProcessBufferHeap();

        void* Alloc(int size);

        void Release(void* bufferToRelease);

    private:
        bool SetSize(int bufferSize);

    private:
        int* mBufferStart;
        int* mNextBuffer;
        int* mBufferEnd;
    };


    ProcessBufferHeap::ProcessBufferHeap(int bufferSize): mBufferStart(NULL)
                                            ,mNextBuffer(NULL)
                                            ,mBufferEnd(NULL)
    {
        SetSize(bufferSize);
    }

    ProcessBufferHeap::~ProcessBufferHeap()
    {
        delete[] (mBufferStart - 1);
    }

    bool ProcessBufferHeap::SetSize(int bufferSize)
    {
        bufferSize = (bufferSize + sizeof(int)-1 )/ sizeof(int);

        mBufferStart = new int[bufferSize + 1];
        if (mBufferStart)
        {
            mBufferStart[0] = 0;
            ++mBufferStart;
            mNextBuffer = mBufferStart;
            mBufferEnd = mBufferStart + bufferSize;

            return true;
        }
        return false;
    }

    void* ProcessBufferHeap::Alloc(int size)
    {
        int heapSize = (size + (sizeof(int)-1))/sizeof(int) + 2;
        if (heapSize < mBufferEnd - mNextBuffer)
        {
            int* buffer = mNextBuffer;
            buffer[0] = heapSize;

            mNextBuffer += heapSize;
            mNextBuffer[-1] = heapSize;

            return buffer+1;
        }
        else
        {
            unsigned char* buffer = new unsigned char[size];
            return buffer;
        }
    }

    void ProcessBufferHeap::Release(void* bufferToRelease)
    {

        if (reinterpret_cast<int*>(bufferToRelease) < mBufferStart
            || reinterpret_cast<int*>(bufferToRelease) > mBufferEnd)
        {
            delete[] reinterpret_cast<unsigned char*>(bufferToRelease);
            return;
        }

        int* buffer = reinterpret_cast<int*>(bufferToRelease) - 1;
        int size = -buffer[0];
        buffer[0] = size;
        buffer[-1 - size] = size;

        int size2 = mNextBuffer[-1];
        if (size2 < 0)
        {
            mNextBuffer += size2;
            size2 = mNextBuffer[-1];
            if (size2 < 0)
            {
                mNextBuffer += size2;
            }
        }
        else
        {
            size2 = buffer[-1];
            if (size2 < 0)
            {
                size += size2;
                buffer += size2;
                buffer[0] = size;
                buffer[-1 - size] = size;
            }

            size2 = buffer[-size];
            if (size2 < 0)
            {
                size += size2;
                buffer[0] = size;
                buffer[-1-size] = size;
            }
        }
    }
}


ProcessBufferHeap g_ProcessBufferHeap(1024*16);

void* allocProcessBuffer(int size)
{
    return g_ProcessBufferHeap.Alloc(size);
}

void releaseProcessBuffer(void* buffer)
{
    g_ProcessBufferHeap.Release(buffer);
}