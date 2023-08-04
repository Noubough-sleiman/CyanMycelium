/*
   This file hold a specific implementation of underlying RTOS equivalent queue.
   It leverage the Fifo implementation located into Collections.
*/

#ifndef _CM_WIN64_EMBEDDED_QUEUE__
#define _CM_WIN64_EMBEDDED_QUEUE__

#include <windows.h>
#include <mutex>
#include <condition_variable>
#include "collections/cm_fifo.hpp"

namespace CyanMycelium
{

    class EmbeddedQueue
    {
    public:
        EmbeddedQueue(int size, int elemSize);
        ~EmbeddedQueue();
        void Reset();
        bool Send(void *item, unsigned int timeoutMs = INFINITE);
        bool Receive(void *o_item, unsigned int timeoutMs = INFINITE);
        bool Peek(void *o_item, unsigned int timeoutMs = INFINITE);
        unsigned int FreeSize();
        unsigned int Size();
        bool ISR_Send(void *item);

    private:
        Fifo _queue;
        std::mutex _mtx;
        std::condition_variable _cv;
    };

    typedef EmbeddedQueue *EmbeddedQueuePtr;
}
#endif