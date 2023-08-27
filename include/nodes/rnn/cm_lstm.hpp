#ifndef _CM_NODE_LSTM_
#define _CM_NODE_LSTM_
#include "cm_graph.hpp"

namespace CyanMycelium
{
  class LSTM : public Operator
  {
  public:
    LSTM() : Operator(){};
    bool Activate(IActivationCtxPtr ctx) override { return true; }
  };
}
#endif