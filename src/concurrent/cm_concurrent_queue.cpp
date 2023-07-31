#include "concurrent/cm_concurrent_queue.hpp"

using namespace CyanMycelium;

template <typename T>
ConcurrentQueue<T> :: ConcurrentQueue(Queue<T> * q, unsigned int to = CM_DEFAULT_CC_TIMEOUT)
{
    _queue = q;
    _wait = new Semaphore(0,_queue->Capacity());
    _lock = new Mutex();
    _timeout = to;
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
    
}

template <typename T>
void ConcurrentQueue<T> :: Stop(bool wait = false)
{
    
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
void ConcurrentQueue<T> :: Run()
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
    
    } while(true);
}