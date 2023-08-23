#include "collections/cm_stack.hpp"
#include "cm.h"

using namespace CyanMycelium;

Stack::Stack(int capacity, int itemSize) // constructeur
{
    _capacity = capacity;
    _itemSize = itemSize;
    _front = 0;
    _rear = -1;
    _count = 0;
    this->_arr = (unsigned char *)cm_malloc(capacity * itemSize);
}
Stack::~Stack() // destructeur
{
    free(_arr);
}

bool Stack::TryDequeue(void *target)
{
    if (IsEmpty())
    {
        return false;
    }

    cm_memcpy(target, _arr + (_rear * _itemSize), _itemSize);
    _rear = (_rear - 1) % _capacity;
    _count--;
    return true;
}

bool Stack::TryEnqueue(void *item)
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

bool Stack::TryPeek(void *target)
{
    if (IsEmpty())
    {
        return false;
    }
    cm_memcpy(target, _arr + (_rear * _itemSize), _itemSize);
    return true;
}
