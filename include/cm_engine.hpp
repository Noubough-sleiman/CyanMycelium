#ifndef __CM_ENGINE__
#define __CM_ENGINE__

#include "memory/cm_memory_manager.hpp"
#include "cm_session.hpp"
#include "concurrent/cm_task.hpp"

namespace CyanMycelium
{
#define CM_DEFAULT_CQ_TIMEOUT 5000
#define CM_DEFAULT_CQ_NTHREAD 2
#define CM_DEFAULT_CQ_STACKSIZE 0
#define CM_DEFAULT_CQ_PRIORITY Thread ::Priority::MEDIUM
#define CM_DEFAULT_CQ_CAPACITY 32

  struct InferenceEngineOptions
  {
    int QueueCapacity = CM_DEFAULT_CQ_CAPACITY;
    int ThreadCount = CM_DEFAULT_CQ_NTHREAD;
    int WaitTimeout = CM_DEFAULT_CQ_TIMEOUT;
    int StackSize = CM_DEFAULT_CQ_STACKSIZE;
    Thread ::Priority Priority = CM_DEFAULT_CQ_PRIORITY;
    IRunnable *Runtime = nullptr;
    IMemoryManagerPtr MemoryManager = nullptr;
  };

  class InferenceEngine : IRunnable
  {
  public:
    InferenceEngine(InferenceEngineOptions options, boolean autoStart = true) : _queue(options.QueueCapacity, sizeof(ActivationEvent)), _lock()
    {
      _options = options;
      if (autoStart)
      {
        this->Start();
      }
    };

    virtual ~InferenceEngine()
    {
      Stop();
    };

    IMemoryManagerPtr GetMemoryManager()
    {
      return _options.MemoryManager ? _options.MemoryManager : &MemoryManagerBase::Shared();
    };

    AsyncActivationContext *CreateInferenceSession(GraphPtr model, ActivationContextHandlersPtr handlers = nullptr);
    void Start();
    void Stop();
    bool IsStarted();
    void Join();

    unsigned long Run(void *) override;
    void Consume(ActivationEvent &e);

  private:
    Queue _queue;
    Mutex _lock;
    InferenceEngineOptions _options;
    ThreadPtr *_threads;
    bool _started;
  };

  typedef InferenceEngine *InferenceEnginePtr;

}
#endif