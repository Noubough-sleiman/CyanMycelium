/* This file hold a specific implementation of underlying RTOS equivalent queue */

#ifndef __CM_COLLECTIONS_STACK__
#define __CM_COLLECTIONS_STACK__

namespace CyanMycelium
{
    class Stack
    {
    public:
        Stack(int capacity, int itemSize); // constructeur
        ~Stack();
        bool TryDequeue(void *);
        bool TryEnqueue(void *);
        bool TryPeek(void *);

        int Count()
        {
            return _count;
        }
        int Capacity()
        {
            return _capacity;
        }
        bool IsEmpty()
        {
            return (_count == 0);
        }
        bool HasItems()
        {
            return (_count != 0);
        }
        bool IsFull()
        {
            return (_count == _capacity);
        }

    private:
        unsigned char *_arr;
        int _capacity; // maximum capacity
        int _itemSize; // item size
        int _front;    // front element index into the circular buffer
        int _rear;     // rear element index into the circular buffer
        int _count;    // number of items
    };
}
#endif