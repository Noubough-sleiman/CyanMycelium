#include "cm_graph.hpp"
#include "nodes/math/cm_mean.hpp"

namespace CyanMycelium
{
  template <typename T>
  void OP_FUNC_NAME(Mean)(TensorPtr x, TensorPtr out, UnaryOperatorPtr node)
  {
    T *data = static_cast<T *>(x->Data);
    T *res = static_cast<T *>(out->Data);
    T sum = data[0];
    for (size_t i = 1; i < x->Count; ++i)
    {
      sum += data[i];
    }
    res[0] = sum / x->Count;
  };

  UNARY_OP_ARRAY_IMPL(Mean,
                      nullptr,                            // Placeholder for TDT_UNDEFINED
                      UNARY_FUNCTION_PTR(Mean, float),    // Function for TDT_FLOAT
                      UNARY_FUNCTION_PTR(Mean, uint8_t),  // Function for TDT_UINT8
                      UNARY_FUNCTION_PTR(Mean, int8_t),   // Function for TDT_INT8
                      UNARY_FUNCTION_PTR(Mean, uint16_t), // Function for TDT_UINT16
                      UNARY_FUNCTION_PTR(Mean, int16_t),  // Function for TDT_INT16
                      UNARY_FUNCTION_PTR(Mean, int32_t),  // Function for TDT_INT32
                      UNARY_FUNCTION_PTR(Mean, int64_t),  // Function for TDT_INT64
                      nullptr,                            // Function for TDT_STRING
                      nullptr,                            // Function for TDT_BOOL
                      nullptr,                            // Function for TDT_FLOAT16
                      UNARY_FUNCTION_PTR(Mean, double),   // Function for TDT_DOUBLE
                      UNARY_FUNCTION_PTR(Mean, uint32_t), // Function for TDT_UINT32
                      UNARY_FUNCTION_PTR(Mean, uint64_t), // Function for TDT_UINT64
                      nullptr,                            // Function for TDT_COMPLEX64
                      nullptr,                            // Function for TDT_COMPLEX128
                      nullptr,                            // Function for TDT_BFLOAT16
                      nullptr,                            // Function for TDT_FLOAT8E4M3FN
                      nullptr,                            // Function for TDT_FLOAT8E4M3FNUZ
                      nullptr,                            // Function for TDT_FLOAT8E5M2
                      nullptr);                           // Function for TDT_FLOAT8E5M2FNUZ
}