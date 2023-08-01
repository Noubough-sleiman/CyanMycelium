#ifndef __CYAN_MISCELIUM_CONCURRENT_QUEUE__
#define	__CYAN_MISCELIUM_CONCURRENT_QUEUE__

#include "collections/cm_queue.hpp"
#include "concurrent/cm_concurrent.hpp"
#include "concurrent/cm_task.hpp"

namespace CyanMycelium
{
   #define CM_DEFAULT_CQ_TIMEOUT 5000
   #define CM_DEFAULT_CQ_NTHREAD 2
   #define CM_DEFAULT_CQ_STACKSIZE 0
   #define CM_DEFAULT_CQ_PRIORITY 0


   struct ConcurrentQueueOptions
   {
     unsigned int ThreadCount;
     unsigned int WaitTimeout;
     unsigned int StackSize;
     Thread :: Priority Priority;
   };

   template <typename T>
    class ConcurrentQueue
    {
     public:
     ConcurrentQueue(Queue<T> *, ConcurrentQueueOptions * options);
      ~ConcurrentQueue();

     void Start();
     void Stop(bool=false);
     bool TryProduce(T);
     virtual void Consume(T) = 0;

     private:
     unsigned long _Run(void *);
     bool _IsStartedSafe();

     ConcurrentQueueOptions _options;

     Queue<T> * _queue;
     
     
     // sync
     SemaphorePtr _wait;
     MutexPtr _lock;

     // threading
     ThreadPtr _threads;
     bool _started;
    };

}
#endif