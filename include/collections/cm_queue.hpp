#ifndef __CYAN_MISCELIUM_QUEUE__
#define	__CYAN_MISCELIUM_QUEUE__

namespace CyanMycelium
{
    #define CM_DEFAULT_QUEUE_SIZE  32

    template <typename T>
    class Queue
    {

    public:
      Queue(int capacity = CM_DEFAULT_QUEUE_SIZE) ; // constructeur
      ~Queue(); // destructeur
 
      bool TryDequeue(T *);
      bool TryEnqueue(T);
      bool TryPeek(T*);
      int Count();      
      int Capacity();
      bool IsEmpty();
      bool IsFull();

    private:
      T * _arr;       
      int _capacity ;  // maximum capacity
      int _front;      // front element index into the circular buffer
      int _rear;       // rear element index into the circular buffer
      int _count;      // number of items
    };
}
#endif