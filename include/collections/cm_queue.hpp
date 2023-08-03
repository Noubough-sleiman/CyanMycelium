#ifndef __CYAN_MISCELIUM_QUEUE__
#define	__CYAN_MISCELIUM_QUEUE__

namespace CyanMycelium
{
    #define CM_DEFAULT_QUEUE_SIZE  32

    template <typename T>
    class Queue
    {

    public:
      Queue(int capacity = CM_DEFAULT_QUEUE_SIZE) // constructeur
      {
          capacity = capacity;
          _front = 0;
          _rear = -1;
          _count = 0;
          _arr = new T[capacity];
      }      
      ~Queue() // destructeur
      {
          delete _arr;
      }
 
      bool TryDequeue(T * target)
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

      bool TryEnqueue(T item)
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

      bool TryPeek(T* target)
      {
        if (IsEmpty())
        {
          *target = nullptr;
          return false;
        }
        *target = _arr[_front];
        return true;
      }
 
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
      
      bool IsFull()
      {
        return (_count == _capacity);
      }

    private:
      T * _arr;       
      int _capacity ;  // maximum capacity
      int _front;      // front element index into the circular buffer
      int _rear;       // rear element index into the circular buffer
      int _count;      // number of items
    };
}
#endif