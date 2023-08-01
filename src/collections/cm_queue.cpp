#include "collections/cm_queue.hpp"

using namespace CyanMycelium;

template <typename T>
Queue<T>::Queue(int capacity )
{
    capacity = capacity;
    _front = 0;
    _rear = -1;
    _count = 0;
    _arr = new T[capacity];
}

template <typename T>
Queue<T>::~Queue() {
    delete _arr;
}
 
template <typename T>
bool Queue<T>::TryDequeue(T * target)
{
    if (IsEmpty())
    {
        *target = nullptr;
        return false;
    }
 
    *target = _arr[_front];
    _front = (_front + 1) % _capacity;
    _count--;
    return true;
}
 
template <typename T>
bool Queue<T>::TryEnqueue(T item)
{
    if (IsFull())
    {
        return false;
    }

    _rear = (_rear + 1) % _capacity;
    _arr[_rear] = item;
    _count++;
    return true;
}
 
template <typename T>
bool Queue<T>::TryPeek(T * target)
{
    if (IsEmpty())
    {
        *target = nullptr;
        return false;
    }
    *target = _arr[_front];
    return true;
}
 
template <typename T>
int Queue<T>::Count() {
    return _count;
}

template <typename T>
int Queue<T>::Capacity() {
    return _capacity;
}
 
template <typename T>
bool Queue<T>::IsEmpty() {
    return (_count== 0);
}
 
template <typename T>
bool Queue<T>::IsFull() {
    return (_count == _capacity);
}