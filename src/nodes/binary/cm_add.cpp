#include "cm_graph.hpp"
#include "nodes/binary/cm_binary.hpp"

namespace CyanMycelium         
{
 #define ADD_CODE(a,b, n) (a + b)
  
 BINARY_FUNC_TEMPLATE(ADD)

 BINARY_OP_ARRAY_IMPL(ADD,
    nullptr,                         // Placeholder for TDT_UNDEFINED
    UNARY_FUNCTION_PTR(ADD,float),   // Function for TDT_FLOAT
    UNARY_FUNCTION_PTR(ADD,uint8_t), // Function for TDT_UINT8
    UNARY_FUNCTION_PTR(ADD,int8_t),  // Function for TDT_INT8
    UNARY_FUNCTION_PTR(ADD,uint16_t),// Function for TDT_UINT16
    UNARY_FUNCTION_PTR(ADD,int16_t), // Function for TDT_INT16
    UNARY_FUNCTION_PTR(ADD,int32_t), // Function for TDT_INT32
    UNARY_FUNCTION_PTR(ADD,int64_t), // Function for TDT_INT64
    nullptr,                         // Function for TDT_STRING
    nullptr,                         // Function for TDT_BOOL
    nullptr,                         // Function for TDT_FLOAT16
    UNARY_FUNCTION_PTR(ADD,double),  // Function for TDT_DOUBLE
    UNARY_FUNCTION_PTR(ADD,uint32_t),// Function for TDT_UINT32
    UNARY_FUNCTION_PTR(ADD,uint64_t),// Function for TDT_UINT64
    nullptr,                         // Function for TDT_COMPLEX64
    nullptr,                         // Function for TDT_COMPLEX128
    nullptr,                         // Function for TDT_BFLOAT16
    nullptr,                         // Function for TDT_FLOAT8E4M3FN
    nullptr,                         // Function for TDT_FLOAT8E4M3FNUZ
    nullptr,                         // Function for TDT_FLOAT8E5M2
    nullptr);                        // Function for TDT_FLOAT8E5M2FNUZ
}