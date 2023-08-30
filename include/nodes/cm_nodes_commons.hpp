
#ifndef _CM_NODE_COMMONS__
#define _CM_NODE_COMMONS__

namespace CyanMycelium
{

#define OP_FUNC_NAME(fname) fname##Func

  ///////////
  // UNARY //
  ///////////

#define UNARY_FUNC_TEMPLATE(fname)                                         \
  template <typename T>                                                    \
  void OP_FUNC_NAME(fname)(Tensor * x, Tensor * out, UnaryOperator * node) \
  {                                                                        \
    T *data = static_cast<T *>(x->Data);                                   \
    T *res = static_cast<T *>(out->Data);                                  \
    for (size_t i = 0; i < x->Count; ++i)                                  \
    {                                                                      \
      T a = data[i];                                                       \
      res[i] = fname##_CODE(a);                                            \
    }                                                                      \
  };

#define UNARY_FUNC_TEMPLATE_WITH_NODE(fname)                               \
  template <typename T>                                                    \
  void OP_FUNC_NAME(fname)(Tensor * x, Tensor * out, UnaryOperator * node) \
  {                                                                        \
    T *data = static_cast<T *>(x->Data);                                   \
    T *res = static_cast<T *>(out->Data);                                  \
    fname *casted = static_cast<fname *>(node);                            \
    for (size_t i = 0; i < x->Count; ++i)                                  \
    {                                                                      \
      T a = data[i];                                                       \
      res[i] = fname##_CODE(a, casted);                                    \
    }                                                                      \
  };

#define UNARY_FUNCTION_PTR(fname, type) OP_FUNC_NAME(fname)<type>

#define UNARY_OP_ARRAY_NAME(fname) fname##FunctionArray
#define UNARY_OP_ARRAY_DECL(fname) extern const UnaryFunctionPtr UNARY_OP_ARRAY_NAME(fname)[TDT_COUNT];
#define UNARY_OP_ARRAY_IMPL(fname, ...) const UnaryFunctionPtr UNARY_OP_ARRAY_NAME(fname)[TDT_COUNT] = {__VA_ARGS__}

#define UNARY_OP_CLASS_DECL(name, ...)                   \
  class name : public UnaryOperator                      \
  {                                                      \
  public:                                                \
    __VA_ARGS__                                          \
    name() : UnaryOperator(UNARY_OP_ARRAY_NAME(name)){}; \
  };                                                     \
  typedef name *name##Ptr;

#define UNARY_OP_DECL(name, ...) UNARY_OP_ARRAY_DECL(name) UNARY_OP_CLASS_DECL(name, __VA_ARGS__)

  ////////////
  // BINARY //
  ////////////

#define BINARY_FUNC_TEMPLATE(fname)                                                     \
  template <typename T>                                                                 \
  void OP_FUNC_NAME(fname)(Tensor * x, Tensor * y, Tensor * out, BinaryOperator * node) \
  {                                                                                     \
    T *xdata = static_cast<T *>(x->Data);                                               \
    T *ydata = static_cast<T *>(y->Data);                                               \
    T *res = static_cast<T *>(out->Data);                                               \
    if (x->AreShapesEqual(y))                                                           \
    {                                                                                   \
      for (size_t i = 0; i < x->Count; ++i)                                             \
      {                                                                                 \
        T a = xdata[i];                                                                 \
        T b = ydata[i];                                                                 \
        res[i] = fname##_CODE(a, b);                                                    \
      }                                                                                 \
    }                                                                                   \
  };

#define BINARY_FUNC_TEMPLATE_WITH_NODE(fname)                                           \
  template <typename T>                                                                 \
  void OP_FUNC_NAME(fname)(Tensor * x, Tensor * y, Tensor * out, BinaryOperator * node) \
  {                                                                                     \
    T *xdata = static_cast<T *>(x->Data);                                               \
    T *ydata = static_cast<T *>(y->Data);                                               \
    T *res = static_cast<T *>(out->Data);                                               \
    if (x->AreShapesEqual(y))                                                           \
    {                                                                                   \
      fname *casted = static_cast<fname *>(node);                                       \
      for (size_t i = 0; i < x->Count; ++i)                                             \
      {                                                                                 \
        T a = xdata[i];                                                                 \
        T b = ydata[i];                                                                 \
        res[i] = fname##_CODE(a, b, casted);                                            \
      }                                                                                 \
    }                                                                                   \
  };

#define BINARY_FUNCTION_PTR(fname, type) OP_FUNC_NAME(fname)<type>

#define BINARY_OP_ARRAY_NAME(fname) fname##FunctionArray
#define BINARY_OP_ARRAY_DECL(fname) extern const BinaryFunctionPtr BINARY_OP_ARRAY_NAME(fname)[TDT_COUNT];
#define BINARY_OP_ARRAY_IMPL(fname, ...) const BinaryFunctionPtr BINARY_OP_ARRAY_NAME(fname)[TDT_COUNT] = {__VA_ARGS__}

#define BINARY_OP_CLASS_DECL(name, ...)                    \
  class name : public BinaryOperator                       \
  {                                                        \
  public:                                                  \
    __VA_ARGS__                                            \
    name() : BinaryOperator(BINARY_OP_ARRAY_NAME(name)){}; \
  };

#define BINARY_OP_DECL(name, ...) BINARY_OP_ARRAY_DECL(name) BINARY_OP_CLASS_DECL(name, __VA_ARGS__)
}
#endif