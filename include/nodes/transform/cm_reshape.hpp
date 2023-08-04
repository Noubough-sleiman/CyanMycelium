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
    bool Activate() override;
  };
}
#endif