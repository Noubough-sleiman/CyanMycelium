#include "nodes/op/cm_shape.hpp"

using namespace CyanMycelium;

#define __ENSURE_OFFSET_POSITIV(a, d) a < 0 ? d + a : a

bool Shape ::Activate(IActivationCtxPtr ctx)
{
    uint64_t *shape = this->Opsc[0]->Payload.Shape;
    int a = _mask.bits._hasStart ? this->_start : 0;
    int b = _mask.bits._hasEnd ? this->_end : this->Opsc[0]->Payload.Dimension - 1;
    a = __ENSURE_OFFSET_POSITIV(a,this->Opsc[0]->Payload.Dimension);
    b = __ENSURE_OFFSET_POSITIV(b,this->Opsc[0]->Payload.Dimension);
    uint64_t oneDimShape[1];
    oneDimShape[0] = (uint64_t)(b - a + 1);

    int count = this->Onsc.Count();
    for (int i = 0; i != count; i++)
    {
        // prepare the link
        Link *l = this->Onsc[i];
        l->Payload.Set(oneDimShape, 1, TDT_UINT32);
        l->Payload.Data = shape + a;
    }
    return true;
}
#undef __ENSURE_OFFSET_POSITIV

bool Shape ::TrySetAtt(const char *n, Att_value_t v)
{
    if (strcmp(n, "Start"))
    {
        _start = v.i;
        _mask.bits._hasStart = 1;
        return true;
    }
    if (strcmp(n, "End"))
    {
        _end = v.i;
        _mask.bits._hasStart = 1;
        return true;
    }
    return false;
}
