#ifndef __CM_SESSION__
#define __CM_SESSION__

#include "cm_graph.hpp"
#include "types/cm_guid.hpp"
#include "concurrent/cm_queue.hpp"

namespace CyanMycelium
{
#define CM_SESSION_WAIT_QUEUE_SIZE 8

  enum ActivationEventType
  {
    CM_ACTIVATION_LINK,
    CM_ACTIVATION_NODE,
    CM_ACTIVATION_OUTPUT
  };

  struct ActivationEvent
  {
    ActivationEventType Type;
    ActivationContext *Context;
    void *Content;
  };

  class AsyncActivationContext : public ActivationContext
  {
  public:
    AsyncActivationContext(InferenceEngine *engine, GraphPtr model, Queue *queue, ActivationContextHandlersPtr handlers = nullptr) : ActivationContext(engine, model, handlers)
    {
      _queue = queue;
    };

  protected:
    bool Activate(OperatorPtr) override;

  private:
    Queue *_queue;
  };
}
#endif