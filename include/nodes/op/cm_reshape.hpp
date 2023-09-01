#ifndef _CM_NODE_RESHAPE_
#define _CM_NODE_RESHAPE_
#include "cm_graph.hpp"

namespace CyanMycelium
{
    class Reshape : public Operator
    {
    public:
        Reshape() : Operator(){};
        bool Activate(ActivationContext *ctx) override { return true; }
    };
}
#endif