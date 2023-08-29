#ifndef __CM_SESSION__
#define __CM_SESSION__

#include "cm_graph.hpp"
#include "types/cm_guid.hpp"
#include "concurrent/cm_queue.hpp"

namespace CyanMycelium
{
#define CM_SESSION_WAIT_QUEUE_SIZE 8

  typedef KeyValueCollection<void *> DataCollections;

  class InferenceSession : public IActivationCtx
  {
  public:
    InferenceSession(GraphPtr model, Queue *queue, IMemoryManagerPtr mm) : IActivationCtx(mm), _wait(CM_SESSION_WAIT_QUEUE_SIZE, sizeof(void *))
    {
      _model = model;
      _queue = queue;
    };

    TensorPtr GetInputInfos(const char *name);
    TensorPtr GetOutputInfos(const char *name = nullptr);
    GraphPtr GetModel() { return _model; }

    bool RunAsync(DataCollections *, DataCollections *);
    DataCollections *AwaitOutputs(int timeout = CM_INFINITE);

    // IActivation context implementation
    bool Activate(LinkPtr) override;
    bool Activate(OperatorPtr) override;

  private:
    GraphPtr _model;
    Queue *_queue;
    Queue _wait;
    DataCollections *_target;
  };

  typedef InferenceSession *InferenceSessionPtr;
}
#endif