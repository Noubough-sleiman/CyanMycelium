#ifndef _CM_CONCURRENT_QUEUE__
#define _CM_CONCURRENT_QUEUE__

#include "cm.h"

namespace CyanMycelium
{

    class Queue
    {
    public:
        Queue(int size, int elemSize);
        ~Queue();

        void Reset();
        boolean Send(void *item, unsigned int timeoutMs = CM_INFINITE);
        boolean Receive(void *o_item, unsigned int timeoutMs = CM_INFINITE);
        boolean Peek(void *o_item, unsigned int timeoutMs = CM_INFINITE);
        unsigned int FreeSize();
        unsigned int Size();
        bool ISR_Send(void *item);

    private:
        cm_queue_handle_t _queue;
    };

}
#endif