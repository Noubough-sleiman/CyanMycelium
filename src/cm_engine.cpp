#include "cm_engine.hpp"

using namespace CyanMycelium;

AsyncActivationContext *InferenceEngine ::CreateSession(GraphPtr model, ActivationContextHandlersPtr handlers)
{
  if (!model)
  {
    return nullptr;
  }
  return new AsyncActivationContext(this, model, &(this->_queue), handlers);
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

void InferenceEngine::Join()
{
  _lock.Take();
  if (_started)
  {
    for (int i = 0; i < _options.ThreadCount; ++i)
    {
      if (_threads[i] && _threads[i]->joinable())
      {
        _threads[i]->join();
        delete _threads[i]; // Clean up the thread pointer
        _threads[i] = nullptr;
      }
    }
    delete[] _threads; // Clean up the array of thread pointers
    _threads = nullptr;
    _started = false;
  }
  _lock.Give();
}

void InferenceEngine ::Stop()
{
  _started = false;
  Join();
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
