/*
  Specific Queue implementation for WIN64.
  WARNING - thi sis only an utility class while this mimic the functionality but
  this not copy the exact behavior as the queue is passing the object by reference and DO NOT
  copy the object. Producer and Consumer may take this in consideration.
*/

#include "concurrent/cm_embedded_queue_win64.hpp"

using namespace CyanMycelium;

EmbeddedQueue::EmbeddedQueue(int size, int elemSize) : _queue(size, elemSize)
{
}

EmbeddedQueue::~EmbeddedQueue()
{
}

void EmbeddedQueue::Reset()
{
}

bool EmbeddedQueue::Send(void *item, unsigned int timeoutMs)
{
    {
        std::lock_guard lk(_mtx);
        if (!_queue.TryEnqueue(item))
        {
            return false;
        }
    }
    _cv.notify_one();
    return true;
}

bool EmbeddedQueue::Receive(void *o_item, unsigned int timeoutMs)
{
    std::unique_lock lk(_mtx);
    if (_cv.wait_for(lk, std::chrono::milliseconds(timeoutMs), [this]() -> bool
                     { return this->_queue.HasItems(); }))
    {
        return _queue.TryDequeue(o_item);
    }
    return false;
}

bool EmbeddedQueue::Peek(void *o_item, unsigned int timeoutMs)
{
    std::unique_lock lk(_mtx);
    if (_cv.wait_for(lk, std::chrono::milliseconds(timeoutMs), [this]() -> bool
                     { return this->_queue.HasItems(); }))
    {
        return _queue.TryPeek(o_item);
    }
    return false;
}

unsigned int EmbeddedQueue::FreeSize()
{
    std::lock_guard lk(_mtx);
    return _queue.Capacity() - _queue.Count();
}

unsigned int EmbeddedQueue::Size()
{
    std::lock_guard lk(_mtx);
    return _queue.Count();
}

bool EmbeddedQueue::ISR_Send(void *item)
{
    return Send(item);
}