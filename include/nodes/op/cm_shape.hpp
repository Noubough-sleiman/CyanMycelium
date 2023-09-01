#ifndef _CM_NODE_RESHAPE_
#define _CM_NODE_RESHAPE_
#include "cm_graph.hpp"

namespace CyanMycelium
{
    class Shape : public Operator
    {

    public:
        Shape() : Operator()
        {
            _mask.value = 0;
        };

        bool Activate(ActivationContext *ctx) override;
        bool TrySetAtt(const char *n, Att_value_t v) override;

    private:
        union
        {
            struct
            {
                unsigned char _hasStart : 1;
                unsigned char _hasEnd : 1;
            } bits;
            unsigned char value;
        } _mask;
        int _start;
        int _end;
    };
}
#endif