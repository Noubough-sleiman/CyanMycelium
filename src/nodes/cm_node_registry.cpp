#include "nodes/cm_nodes_registry.hpp"
#include "nodes/unary/cm_unary.hpp"
#include "nodes/unary/cm_celu.hpp"
#include "nodes/binary/cm_binary.hpp"
#include "nodes/math/cm_mean.hpp"
#include "nodes/rnn/cm_lstm.hpp"
#include "nodes/op/cm_concat.hpp"
#include "nodes/op/cm_reshape.hpp"

using namespace CyanMycelium;

NodeRegistry NodeRegistry::__Shared;

Operator * NodeRegistry::ForName(const char *n)
{
    NodeInitializer_fn initializer = __Shared._types.Get(n);
    if (initializer)
    {
        return initializer();
    }
    return nullptr;
}

NodeRegistry::NodeRegistry(int initialCapacity) : _types(initialCapacity)
{
    // unary
    __REGISTER__NODE(Abs);
    /*   __REGISTER__NODE(ACOS);
       __REGISTER__NODE(ACOSH);
       __REGISTER__NODE(ASIN);
       __REGISTER__NODE(ASINH);
       __REGISTER__NODE(ATAN);
       __REGISTER__NODE(ATANH);
       __REGISTER__NODE(CEIL);
       __REGISTER__NODE(COS);
       __REGISTER__NODE(COSH);
       __REGISTER__NODE(ELU);
       __REGISTER__NODE(EXP);
       __REGISTER__NODE(FLOOR);
       __REGISTER__NODE(CELU);

       // binary
       __REGISTER__NODE(ADD);
       __REGISTER__NODE(AND);
       __REGISTER__NODE(DIV);
       __REGISTER__NODE(EQUAL);
       __REGISTER__NODE(GREATER);
       __REGISTER__NODE(LESS);
       __REGISTER__NODE(MAX);
       __REGISTER__NODE(MEAN);
       __REGISTER__NODE(MIN);
       __REGISTER__NODE(MULT);
       __REGISTER__NODE(OR);
       __REGISTER__NODE(POW);
       __REGISTER__NODE(SUB);
       __REGISTER__NODE(WHERE);
       __REGISTER__NODE(XOR);

       // math
       __REGISTER__NODE(Mean);
*/
    // rnn
    __REGISTER__NODE(LSTM);

    // op
    __REGISTER__NODE(Concat);
    __REGISTER__NODE(Reshape);
};
