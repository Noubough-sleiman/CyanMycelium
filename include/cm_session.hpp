#ifndef __CM_SESSION__
#define __CM_SESSION__

#include "cm_graph.hpp"
#include "types/cm_guid.hpp"
#include "concurrent/cm_queue.hpp"

namespace CyanMycelium
{
  class InferenceSession : public IActivationCtx
  {
  public:
    InferenceSession(GraphPtr model, Queue *queue, IMemoryManagerPtr mm) : IActivationCtx(mm)
    {
      _model = model;
      _queue = queue;
    };

    TensorPtr GetInputInfos(const char *name);
    TensorPtr GetOutputInfos(const char *name = nullptr);

    bool RunAsync(KeyValueCollection<void *> *, KeyValueCollection<void *> *);

    void OnBegin(KeyValueCollection<void *> *) override;
    bool Activate(LinkPtr) override;
    bool Activate(NodePtr) override;
    void OnEnd(KeyValueCollection<void *> *) override;

    GraphPtr GetModel()
    {
      return _model;
    }

  private:
    GraphPtr _model;
    Queue *_queue;
    KeyValueCollection<void *> *_output;
  };

  typedef InferenceSession *InferenceSessionPtr;

}
#endif