#ifndef __CYAN_MISCELIUM_CONCURRENT_QUEUE__
#define	__CYAN_MISCELIUM_CONCURRENT_QUEUE__

#include "collections/cm_queue.hpp"
#include "concurrent/cm_concurrent.hpp"

namespace CyanMycelium
{
   #define CM_DEFAULT_CC_TIMEOUT 5000

   template <typename T>
    class ConcurrentQueue
    {
     public:
     ConcurrentQueue(Queue<T> *, unsigned int = CM_DEFAULT_CC_TIMEOUT);
      ~ConcurrentQueue();

     void Start();
     void Stop(bool=false);
     bool TryProduce(T);
     void Run();
     virtual void Consume(T) = 0;

     private:
     Queue<T> * _queue;
     SemaphorePtr _wait;
     MutexPtr _lock;
     unsigned int _timeout;
    };

}
#endif