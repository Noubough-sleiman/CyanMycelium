#ifndef _CM_NODE_RESHAPE__
#define _CM_NODE_RESHAPE__
#include "cm_graph.hpp"

namespace CyanMycelium
{
#define ALLOW_ZERO_DEFAULT 0

  /// @brief Reshape the input tensor similar to numpy.reshape.
  /// @link https://onnx.ai/onnx/operators/onnx__Reshape.html
  class Reshape : Operator
  {
  public:
    int8_t AllowZero = ALLOW_ZERO_DEFAULT;
    Reshape() : Operator(){};
    bool TrySetAtt(const char *n, Att_value_t v)
    {
      if (strcmp(n, "AllowZero"))
      {
        AllowZero = (int8_t)v.i;
        return true;
      }
      return false;
    }
  };
}
#endif