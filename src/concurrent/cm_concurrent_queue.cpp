#include "concurrent/cm_concurrent_queue.hpp"

using namespace CyanMycelium;

template <typename T>
ConcurrentQueue<T> :: ConcurrentQueue(Queue<T> * q, ConcurrentQueueOptions * options )
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

template <typename T>
void ConcurrentQueue<T> :: Stop(bool wait)
{
  _started = false;
}

template <typename T>
bool ConcurrentQueue<T> :: TryProduce(T obj)
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

template <typename T>
unsigned long  ConcurrentQueue<T> :: _Run(void *)
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