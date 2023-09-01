#include "cm_engine.hpp"

using namespace CyanMycelium;

AsyncActivationContext *InferenceEngine ::CreateSession(GraphPtr model, IMemoryManagerPtr memoryManager)
{
  return new AsyncActivationContext(model, memoryManager, &_queue);
}

void InferenceEngine ::Start()
{
  // we create the thread
  _lock.Take();
  if (!_started)
  {
    _started = true;
    _threads = new ThreadPtr[_options.ThreadCount];
    IRunnable *runnable = _options.Runtime ? _options.Runtime : this;
    for (int i = 0; i < _options.ThreadCount; i++)
    {
      _threads[i] = new Thread(runnable, _options.StackSize, this, _options.Priority);
    }
  }
  _lock.Give();
}

void InferenceEngine ::Stop()
{
  _started = false;
}

bool InferenceEngine ::IsStarted()
{
  _lock.Take();
  bool tmp = _started;
  _lock.Give();
  return tmp;
}

unsigned long InferenceEngine ::Run(void *)
{
  if (IsStarted())
  {
    ActivationEvent e;
    do
    {
      if (_queue.Receive(&e, _options.WaitTimeout))
      {
        Consume(e);
      }
    } while (IsStarted());
  }
  return 0l;
}

void InferenceEngine ::Consume(ActivationEvent &e)
{
  ActivationContext *context = e.Context;

  switch (e.Type)
  {
  case CM_ACTIVATION_NODE:
  {
    OperatorPtr node = (OperatorPtr)e.Content;
    if (node)
    {
      context->ActivationContext::Activate(node);
    }
    break;
  }
  default:
  {
    break;
  }
  }
}
