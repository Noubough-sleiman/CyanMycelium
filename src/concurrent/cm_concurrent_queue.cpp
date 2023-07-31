#include "concurrent/cm_concurrent_queue.hpp"

using namespace CyanMycelium;

template <typename T>
ConcurrentQueue<T> :: ConcurrentQueue(Queue<T> *, ConcurrentQueueOptions * options )
{
    _queue = q;
    _wait = new Semaphore(0,_queue->Capacity());
    _lock = new Mutex();
    _timeout = to;
    _threadCount = max(nthread,1);
    _threads = nullptr;
    _started = false;
    _options.ThreadCount = options ? options->ThreadCount : CM_DEFAULT_CQ_NTHREAD ;
    _options.WaitTimeout = options ? options->WaitTimeout : CM_DEFAULT_CQ_TIMEOUT ;
    _options.StackSize = options ? options->StackSize : CM_DEFAULT_CQ_STACKSIZE ;
    _options.Priority = options ? options->Priority : CM_DEFAULT_CQ_PRIORITY ;
}

template <typename T>
ConcurrentQueue<T> :: ~ConcurrentQueue()
{
    delete _wait;
    delete _lock;
}

template <typename T>
void ConcurrentQueue<T> :: Start()
{
    // we create the thread
    _lock->Take();
    if( !_started)
    {
      _started = true;
      _threads = new ThreadPtr[_threadCount];
      void * params = null;
      unsigned int stackSize = 0;
      for(int i=0; i < _threadCount; i++)
      {
        _threads[i] = new Thread(_Run,_options.StackSize, params, _options.Priority);
      }
    }
    _lock->Give();
}

template <typename T>
void ConcurrentQueue<T> :: Stop(bool wait = false)
{
   // we 
    
}

template <typename T>
bool ConcurrentQueue<T> :: TryProduce(T obj)
{
    _lock->Take();
    if(_queue->TryEnqueue(T))
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

template <typename T>
unsigned long  ConcurrentQueue<T> :: _Run(void *)
{

    _lock->Take();
    started = _started;
    _lock->Give();
    if( started)
    {
      do
      {
        // we wait until at least on object available
        _wait->Take(_timeout);
    
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

      } while(started)
    }
}