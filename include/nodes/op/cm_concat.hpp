#ifndef _CM_NODE_CONCAT_
#define _CM_NODECONCAT_
#include "cm_graph.hpp"

namespace CyanMycelium
{
    class Concat : public Operator
    {
    public:
        Concat() : Operator(){};
        bool Activate(ActivationContext *ctx) override { return true; }
    };

}
#endif