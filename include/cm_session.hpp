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
    AsyncActivationContext(GraphPtr model, IMemoryManagerPtr mm, Queue *queue) : ActivationContext(model, mm)
    {
      _queue = queue;
    };

    void AwaitOutputs(int timeout = CM_INFINITE);

  protected:
    bool Activate(OperatorPtr) override;
    bool OnOutputReady() override;

  private:
    Queue *_queue;
  };
}
#endif