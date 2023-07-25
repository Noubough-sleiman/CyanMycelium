#ifndef __CYAN_MISCELIUM_NODE_LSTM__
#define __CYAN_MISCELIUM_NODE_LSTM__
#include "./cm_graph.hpp"

namespace CyanMycelium
{
  class LSTM : Operator { 
    public: 
    LSTM() : Operator(){};
    bool Activate() override;
  };

}
#endif