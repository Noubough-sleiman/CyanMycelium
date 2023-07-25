
#ifndef __CYAN_MISCELIUM_NODE_COMMONS__
#define __CYAN_MISCELIUM_NODE_COMMONS__

namespace CyanMycelium
{
  ///////////
  // UNARY //
  ///////////
  
  typedef void (*UnaryFunctionPtr)(TensorPtr, TensorPtr, UnaryOperatorPtr);

  #define UNARY_FUNC_NAME(fname) fname ## Func
  #define UNARY_FUNC(fname) \
  template <typename T> \
    void UNARY_FUNC_NAME(fname) (TensorPtr x, TensorPtr out, UnaryOperatorPtr node) \
    {  \
      T* data = static_cast<T*>(x->Data);  \
      T* res = static_cast<T*>(out->Data);  \
      for (size_t i = 0; i < x->Count; ++i)  \
      {  \
        res[i] = fname##_CODE(a,node) ;  \
      }  \
    }; 

  #define UNARY_FUNCTION_PTR(fname,type) &##UNARY_FUNC_NAME(fname,type)<type>

  #define UNARY_OP_ARRAY_NAME(fname) fname##FunctionArray
  #define UNARY_OP_ARRAY_DECL(fname) extern const UnaryFunctionPtr UNARY_OP_ARRAY_NAME(fname)[TDT_COUNT];
  #define UNARY_OP_ARRAY_IMPL(fname,...) const UnaryFunctionPtr UNARY_OP_ARRAY_NAME(fname)[TDT_COUNT]={__VA_ARGS__}
  
  #define UNARY_OP_CLASS_DECL(name, ...)\
  class name ## : UnaryOperator { \
    public: \
    __VA_ARGS__  \
    name##() : UnaryOperator(UNARY_OP_ARRAY_NAME(name)){ };\
  };

  #define UNARY_OP_DECL(name, ...) UNARY_OP_ARRAY_DECL(name) UNARY_OP_CLASS_DECL(name,__VA_ARGS__)
  
  ////////////
  // BINARY //
  ////////////

  typedef void (*BinaryFunctionPtr)(TensorPtr, TensorPtr, TensorPtr, BinaryOperatorPtr);

  #define BINARY_FUNC_NAME(fname) fname ## Func
  #define BINARY_FUNC(fname) \
  template <typename T> \
    void BINARY_FUNC_NAME(fname) (TensorPtr x, TensorPtr y, TensorPtr out, UnaryOperatorPtr node) \
    {\
      T* xdata =  static_cast<T*>(x->Data);\
      T* ydata =  static_cast<T*>(y->Data);\
      T* res =  static_cast<T*>(out->Data);\
      if( x->ShapesAreEqual(y) )\
      {\
        for (size_t i = 0; i < x->Count; ++i)\
        {\
          res[i] = fname##_CODE(xdata[i],ydata[i],node);\
        }\
      }\
    }; 

  #define BINARY_FUNCTION_PTR(fname,type) &##BINARY_FUNC_NAME(fname,type)<type>

  #define BINARY_OP_ARRAY_NAME(fname) fname##FunctionArray
  #define BINARY_OP_ARRAY_DECL(fname) extern const BinaryFunctionPtr UNARY_OP_ARRAY_NAME(fname)[TDT_COUNT];
  #define BINARY_OP_ARRAY_IMPL(fname,...) const BinaryFunctionPtr UNARY_OP_ARRAY_NAME(fname)[TDT_COUNT]={__VA_ARGS__}
  
  #define BINARY_OP_CLASS_DECL(name, ...)\
  class name ## : BinaryOperator { \
    public: \
    __VA_ARGS__  \
    name##() : BinaryOperator(BINARY_OP_ARRAY_NAME(name)){ };\
  };

  #define BINARY_OP_DECL(name, ...) BINARY_OP_ARRAY_DECL(name) BINARY_OP_CLASS_DECL(name,__VA_ARGS__)
}
#endif