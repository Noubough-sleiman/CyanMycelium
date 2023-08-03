#include "cm_graph.hpp"
#include "nodes/binary/cm_binary.hpp"

namespace CyanMycelium
{
#define AND_CODE(a, b) (a && b)

    BINARY_FUNC_TEMPLATE(AND)

    BINARY_OP_ARRAY_IMPL(AND,
                         nullptr,                        // Placeholder for TDT_UNDEFINED
                         nullptr,                        // Function for TDT_FLOAT
                         nullptr,                        // Function for TDT_UINT8
                         nullptr,                        // Function for TDT_INT8
                         nullptr,                        // Function for TDT_UINT16
                         nullptr,                        // Function for TDT_INT16
                         nullptr,                        // Function for TDT_INT32
                         nullptr,                        // Function for TDT_INT64
                         nullptr,                        // Function for TDT_STRING
                         BINARY_FUNCTION_PTR(AND, bool), // Function for TDT_BOOL
                         nullptr,                        // Function for TDT_FLOAT16
                         nullptr,                        // Function for TDT_DOUBLE
                         nullptr,                        // Function for TDT_UINT32
                         nullptr,                        // Function for TDT_UINT64
                         nullptr,                        // Function for TDT_COMPLEX64
                         nullptr,                        // Function for TDT_COMPLEX128
                         nullptr,                        // Function for TDT_BFLOAT16
                         nullptr,                        // Function for TDT_FLOAT8E4M3FN
                         nullptr,                        // Function for TDT_FLOAT8E4M3FNUZ
                         nullptr,                        // Function for TDT_FLOAT8E5M2
                         nullptr);                       // Function for TDT_FLOAT8E5M2FNUZ
}