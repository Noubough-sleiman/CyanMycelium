/*
  Specific Queue implementation for WIN64.
  WARNING - thi sis only an utility class while this mimic the functionality but
  this not copy the exact behavior as the queue is passing the object by reference and DO NOT
  copy the object. Producer and Consumer may take this in consideration.
*/
#include "concurrent/cm_queue.hpp"

using namespace CyanMycelium;

Queue::Queue(int size, int elemSize)
{
    _queue = new EmbeddedQueue(size, elemSize);
}

Queue::~Queue()
{
    delete _queue;
}

void Queue::Reset()
{
    _queue->Reset();
}

boolean Queue::Send(void *item, unsigned int timeoutMs)
{
    return _queue->Send(item, timeoutMs);
}
boolean Queue::Receive(void *o_item, unsigned int timeoutMs)
{
    return _queue->Receive(o_item, timeoutMs);
}
boolean Queue::Peek(void *o_item, unsigned int timeoutMs)
{
    return _queue->Peek(o_item, timeoutMs);
}
unsigned int Queue::FreeSize()
{
    return _queue->FreeSize();
}
unsigned int Queue::Size()
{
    return _queue->Size();
}
bool Queue::ISR_Send(void *item)
{
    return _queue->ISR_Send(item);
}
