#ifndef _CM_NODE_LSTM__
#define _CM_NODE_LSTM__
#include "cm_graph.hpp"

namespace CyanMycelium
{
  class LSTM : Operator
  {
  public:
    LSTM() : Operator(){};
    bool Activate(IActivationCtxPtr ctx) override;
  };

}
#endif