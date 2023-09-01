#include "cm_engine.hpp"
using namespace CyanMycelium;

void AsyncActivationContext ::AwaitOutputs(int timeoutMillis)
{
  ActivationEvent e;
  do
  {
    cm_clock_t t = cm_clock();
    if (this->_queue->Receive(&e, timeoutMillis))
    {
      if (e.Type == CM_ACTIVATION_OUTPUT)
      {
        break;
      }
    }
    cm_clock_t now = cm_clock();
    t = now - t;
    if (t != (cm_clock_t)CM_INFINITE)
    {
      timeoutMillis -= t;
    }
  } while (timeoutMillis == (int)CM_INFINITE || timeoutMillis > 0);
}

bool AsyncActivationContext ::Activate(OperatorPtr node)
{
  ActivationEvent e = {CM_ACTIVATION_NODE, this, node};
  return this->_queue->Send(&e);
}

bool AsyncActivationContext ::OnOutputReady()
{
  ActivationEvent e = {CM_ACTIVATION_OUTPUT, this, &this->GetModel()->Outputs};
  return this->_queue->Send(&e);
}
