#include <cmath>

#include "cm_graph.hpp"
#include "nodes/unary/cm_unary.hpp"

namespace CyanMycelium
{
#define ACOSH_CODE(a) acosh(a)

  UNARY_FUNC_TEMPLATE(ACOSH)

  // according to onnx documentation, Constrain input and output types to float tensors.
  UNARY_OP_ARRAY_IMPL(ACOSH,
                      nullptr,                           // Placeholder for TDT_UNDEFINED
                      UNARY_FUNCTION_PTR(ACOSH, float),  // Function for TDT_FLOAT
                      nullptr,                           // Function for TDT_UINT8
                      nullptr,                           // Function for TDT_INT8
                      nullptr,                           // Function for TDT_UINT16
                      nullptr,                           // Function for TDT_INT16
                      nullptr,                           // Function for TDT_INT32
                      nullptr,                           // Function for TDT_INT64
                      nullptr,                           // Function for TDT_STRING
                      nullptr,                           // Function for TDT_BOOL
                      nullptr,                           // Function for TDT_FLOAT16
                      UNARY_FUNCTION_PTR(ACOSH, double), // Function for TDT_DOUBLE
                      nullptr,                           // Function for TDT_UINT32
                      nullptr,                           // Function for TDT_UINT64
                      nullptr,                           // Function for TDT_COMPLEX64
                      nullptr,                           // Function for TDT_COMPLEX128
                      nullptr,                           // Function for TDT_BFLOAT16
                      nullptr,                           // Function for TDT_FLOAT8E4M3FN
                      nullptr,                           // Function for TDT_FLOAT8E4M3FNUZ
                      nullptr,                           // Function for TDT_FLOAT8E5M2
                      nullptr);                          // Function for TDT_FLOAT8E5M2FNUZ
}