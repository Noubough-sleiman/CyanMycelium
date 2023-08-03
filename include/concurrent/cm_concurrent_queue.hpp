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
        ConcurrentQueue(Queue<T> *, ConcurrentQueueOptions * options)
        {
          _queue = q;
          _wait = new Semaphore(0,_queue->Capacity());
          _lock = new Mutex();
          _threads = nullptr;
          _started = false;
          _options.ThreadCount = max(options ? options->ThreadCount : CM_DEFAULT_CQ_NTHREAD,1) ;
          _options.WaitTimeout = options ? options->WaitTimeout : CM_DEFAULT_CQ_TIMEOUT ;
          _options.StackSize = options ? options->StackSize : CM_DEFAULT_CQ_STACKSIZE ;
          _options.Priority = options ? options->Priority : (Thread :: Priority) CM_DEFAULT_CQ_PRIORITY ;
        }
      ~ConcurrentQueue()
      {
        delete _wait;
        delete _lock;
      }

      void Start()
      {
        // we create the thread
        _lock->Take();
        if( !_started)
        {
          _started = true;
          _threads = new ThreadPtr[_options.ThreadCount];
          void * params = nullptr;
          unsigned int stackSize = 0;
          for(int i=0; i < _options.ThreadCount; i++)
          {
            _threads[i] = new Thread(_Run,_options.StackSize, params, _options.Priority);
          }
        }
        _lock->Give();
      }      

      void Stop(bool=false)
      {
        _started = false;
      }

      bool TryProduce(T)
      {
        _lock->Take();
        if(_queue->TryEnqueue(obj))
        {
          _lock->Give();
          // we tell than one more object is available
          // the count is limited to the capacity of the 
          // queue
          _wait->Give();
          return true;
        }
        _lock->Give();
        return false;
      }

      virtual void Consume(T) = 0;

    private:

      unsigned long _Run(void *)
      {
        _lock->Take();
        boolean started = _started;
        _lock->Give();
        if( started)
        {
          do
          {
            // we wait until at least on object available
            _wait->Take(_options.WaitTimeout);
        
            T * obj;
            _lock->Take();
            if(!_queue->TryDequeue(obj))
            {
              _lock->Give();
              continue;
            }
            _lock->Give();
          
            // here we supposed to get an object
            Consume(*obj);

            // test for exit the thread.
            _lock->Take();
            started = _started;
            _lock->Give();

          } while(started);
        }
        return 0;
      }      

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