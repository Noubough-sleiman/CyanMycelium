#include "nodes/op/cm_shape.hpp"

using namespace CyanMycelium;

#define __ENSURE_OFFSET_POSITIV(a, d) a < 0 ? d + a : a

bool Shape ::Activate(ActivationContext *ctx)
{
    TensorPtr infos = this->Opsc[0]->GetPayloadInfos();
    uint64_t *shape = infos->Shape;
    uint8_t dimension = infos->Dimension;

    int a = _mask.bits._hasStart ? this->_start : 0;
    int b = _mask.bits._hasEnd ? this->_end : dimension - 1;

    a = __ENSURE_OFFSET_POSITIV(a, dimension);
    b = __ENSURE_OFFSET_POSITIV(b, dimension);

    uint64_t oneDimShape[1];
    oneDimShape[0] = (uint64_t)(b - a + 1);

    int count = this->Onsc.Count();
    for (int i = 0; i != count; i++)
    {
        // prepare the links
        Link *l = this->Onsc[i];
        TensorRefPtr ref = ctx->GetPayloadRef(l->Id);
        if (!ref)
        {
            return false;
        }
        ref->Value.Set(oneDimShape, 1, TDT_UINT32);
        ref->Value.Data = shape + a;
        // do NOT let any subsequent operator modify the value.
        ref->Flags.Bits.ReadOnly = 1;
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
