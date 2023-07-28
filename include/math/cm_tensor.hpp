#ifndef __CYAN_MISCELIUM_TENSOR__
#define __CYAN_MISCELIUM_TENSOR__
#include <cstdint>

namespace CyanMycelium         
{
    #define TENSOR_MAX_DIMENSION 2

    typedef enum {
        TDT_UNDEFINED = 0,
        
        // Basic types. 
        TDT_FLOAT = 1, // float 
        TDT_UINT8 = 2, // uint8_t 
        TDT_INT8 = 3, // int8_t 
        TDT_UINT16 = 4, // uint16_t 
        TDT_INT16 = 5, // int16_t 
        TDT_INT32 = 6, // int32_t 
        TDT_INT64 = 7, // int64_t 
        TDT_STRING = 8, // string 
        TDT_BOOL = 9, // bool 
        
        // IEEE754 half-precision floating-point format (16 bits wide). This format has 1 sign bit, 5 exponent bits, and 10 mantissa bits. 
        TDT_FLOAT16 = 10,
        TDT_DOUBLE = 11,
        TDT_UINT32 = 12,
        TDT_UINT64 = 13,
        TDT_COMPLEX64 = 14, // complex with float32 real and imaginary components 
        TDT_COMPLEX128 = 15, // complex with float64 real and imaginary components 
        
        // Non-IEEE floating-point format based on IEEE754 single-precision floating-point number truncated to 16 bits. 
        // This format has 1 sign bit, 8 exponent bits, and 7 mantissa bits.
        TDT_BFLOAT16 = 16,
        
        // Non-IEEE floating-point format based on papers
        // FP8 Formats for Deep Learning, https://arxiv.org/abs/2209.05433,
        // 8-bit Numerical Formats For Deep Neural Networks, https://arxiv.org/pdf/2206.02915.pdf.
        // Operators supported FP8 are Cast, CastLike, QuantizeLinear, DequantizeLinear.
        // The computation usually happens inside a block quantize / dequantize fused by the runtime.
        TDT_FLOAT8E4M3FN = 17, // float 8, mostly used for coefficients, supports nan, not inf
        TDT_FLOAT8E4M3FNUZ = 18, // float 8, mostly used for coefficients, supports nan, not inf, no negative zero
        TDT_FLOAT8E5M2 = 19, // follows IEEE 754, supports nan, inf, mostly used for gradients
        TDT_FLOAT8E5M2FNUZ = 20, // follows IEEE 754, supports nan, inf, mostly used for gradients, no negative zero
        TDT_COUNT
    } tensor_data_type_t;


    class Tensor {
        public:
        /// @brief build a tensor using shape and dimension
        /// @param shape the shape as an array of the size of each dimension
        /// @param dimension the number of axes or indices required to access the elements of the tensor.
        Tensor(uint32_t * shape, int dimension ) ;
  
        size_t Size;  // size in byte, must be equal to Count * sizeof(type)
        size_t Count; // number of elements
        uint8_t * Data; // byte array
        tensor_data_type_t Type; // type of underlying elements
        uint8_t Dimension; // dimension of tensor
        uint32_t Shape[TENSOR_MAX_DIMENSION]; // shape of tensor. 
       
        bool ShapesAreEqual(TensorPtr other);

        TensorPtr Clone();
    };

    typedef Tensor * TensorPtr;
}

#endif