#include <cmath>

#include "cm_graph.hpp"
#include "nodes/unary/cm_celu.hpp"

namespace CyanMycelium
{
#define CELU_CODE(x, n) (max(0, x) + min(0, (n)->Alpha * (exp(x / (n)->Alpha) - 1)))

  UNARY_FUNC_TEMPLATE_WITH_NODE(CELU)

  // according to onnx documentation, Constrain input and output types to float32 tensors.
  UNARY_OP_ARRAY_IMPL(CELU,
                      nullptr,                         // Placeholder for TDT_UNDEFINED
                      UNARY_FUNCTION_PTR(CELU, float), // Function for TDT_FLOAT
                      nullptr,                         // Function for TDT_UINT8
                      nullptr,                         // Function for TDT_INT8
                      nullptr,                         // Function for TDT_UINT16
                      nullptr,                         // Function for TDT_INT16
                      nullptr,                         // Function for TDT_INT32
                      nullptr,                         // Function for TDT_INT64
                      nullptr,                         // Function for TDT_STRING
                      nullptr,                         // Function for TDT_BOOL
                      nullptr,                         // Function for TDT_FLOAT16
                      nullptr,                         // Function for TDT_DOUBLE
                      nullptr,                         // Function for TDT_UINT32
                      nullptr,                         // Function for TDT_UINT64
                      nullptr,                         // Function for TDT_COMPLEX64
                      nullptr,                         // Function for TDT_COMPLEX128
                      nullptr,                         // Function for TDT_BFLOAT16
                      nullptr,                         // Function for TDT_FLOAT8E4M3FN
                      nullptr,                         // Function for TDT_FLOAT8E4M3FNUZ
                      nullptr,                         // Function for TDT_FLOAT8E5M2
                      nullptr);                        // Function for TDT_FLOAT8E5M2FNUZ
}