#include "cm_engine.hpp"
using namespace CyanMycelium;

bool AsyncActivationContext ::Activate(OperatorPtr node)
{
  ActivationEvent e = {CM_ACTIVATION_NODE, this, node};
  return this->_queue->Send(&e);
}
