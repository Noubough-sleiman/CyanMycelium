#include "cm_engine.hpp"

using namespace CyanMycelium;

AsyncActivationContext *InferenceEngine ::CreateInferenceSession(GraphPtr model, ActivationContextHandlersPtr handlers)
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
    int count = this->_options.ThreadCount;
    ThreadPtr *copy = new ThreadPtr[count];
    for (int i = 0; i < count; i++)
    {
      copy[i] = _threads[i];
    }
    _lock.Give();
    for (int i = 0; i < count; ++i)
    {
      if (copy[i] && copy[i]->joinable())
      {
        copy[i]->join();
      }
    }
  }
  else
  {
    _lock.Give();
  }
}

void InferenceEngine ::Stop()
{
  ActivationEvent e = {CM_ACTIVATION_STOP, nullptr, nullptr};
  // we need to send a stop event to every threads
  for (int i = 0; i < _options.ThreadCount; i++)
  {
    if (!this->_queue.Send(&e))
    {
      // log error
    }
  }
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
  case CM_ACTIVATION_STOP:
  {
    _lock.Take();
    delete[] _threads; // Clean up the array of thread pointers
    _threads = nullptr;
    _started = false;
    _lock.Give();
    break;
  }
  default:
  {
    break;
  }
  }
}
