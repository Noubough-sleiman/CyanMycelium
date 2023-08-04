#include "collections/cm_fifo.hpp"
#include "cm.h"

using namespace CyanMycelium;

Fifo::Fifo(int capacity, int itemSize) // constructeur
{
    _capacity = capacity;
    _itemSize = itemSize;
    _front = 0;
    _rear = -1;
    _count = 0;
    this->_arr = (unsigned char *)cm_malloc(capacity * itemSize);
}
Fifo::~Fifo() // destructeur
{
    free(_arr);
}

bool Fifo::TryDequeue(void *target)
{
    if (IsEmpty())
    {
        return false;
    }

    cm_memcpy(target, _arr + (_front * _itemSize), _itemSize);
    _front = (_front + 1) % _capacity;
    _count--;
    return true;
}

bool Fifo::TryEnqueue(void *item)
{
    if (IsFull())
    {
        return false;
    }
    _rear = (_rear + 1) % _capacity;
    cm_memcpy(_arr + (_rear * _itemSize), item, _itemSize);
    _count++;
    return true;
}

bool Fifo::TryPeek(void *target)
{
    if (IsEmpty())
    {
        return false;
    }
    cm_memcpy(target, _arr + (_front * _itemSize), _itemSize);
    return true;
}
