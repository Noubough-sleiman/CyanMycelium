# Node Design Consideration

When designing classes and subclasses in C++, there are various considerations to keep in mind. One crucial decision involves choosing between virtual functions with polymorphism and inheritance versus using a single class with function pointers for different implementations.

## Virtual Functions (Polymorphism and Inheritance)

**Pros:**

- Polymorphism enables flexible behavior based on the actual type of the object.
- Promotes a clean, modular design with well-defined interfaces.
- Allows easy extension by adding new derived classes without modifying existing code.

**Cons:**

- Slightly larger code size due to the vtable and additional overhead of dynamic dispatch.
- Slight runtime overhead for function calls due to dynamic dispatch.

## One Class with Pointers to Function Implementations

**Pros:**

- Potentially smaller code size compared to virtual functions, as there's no need for vtables and dynamic dispatch.
- Can be faster in some cases, as function calls are direct (no dynamic dispatch overhead).

**Cons:**

- Less flexible than virtual functions; all subclasses need to have the same method signatures.
- Code can be harder to maintain and less modular, especially when the number of functions and subclasses grows.

Ultimately, the choice between the two approaches depends on the specific requirements of the project. If code size is a primary concern, using function pointers may result in smaller code. However, this approach sacrifices flexibility and maintainability. If a clean design, polymorphism, and flexibility are crucial, virtual functions and inheritance provide a better choice, even though they might slightly increase code size and have a small runtime overhead due to dynamic dispatch.

## Explanation for Prioritizing Smaller Code and Minimizing Overhead on Micro-Controller.

When aiming for smaller code size and minimal runtime overhead, the approach of using a single class with pointers to function implementations can be preferred. Here are the reasons behind this decision:

1. **Smaller Code Size**: The function pointer approach can result in a more compact executable. In contrast to the virtual function approach with polymorphism and inheritance, which requires vtables for each class with virtual functions, function pointers reduce code size, especially in scenarios with multiple classes involved.

2. **No Vtable Overhead**: Virtual functions and dynamic dispatch through vtables introduce a slight runtime overhead. This overhead arises from the need to determine the appropriate function to call at runtime, which may result in extra CPU cycles and potential cache misses. By using function pointers, function calls are direct and resolved at compile-time, eliminating this overhead.

3. **Direct Function Calls**: Function pointers allow direct function calls without requiring virtual function lookup. Consequently, this can lead to faster execution times, especially in situations where virtual function lookup becomes a performance bottleneck.

However, it is essential to consider the trade-offs and limitations of the function pointer approach:

1. **Loss of Polymorphism and Flexibility**: Function pointers enforce that all subclasses have the same method signatures, making the implementation fixed. This reduces the flexibility and polymorphism achievable through virtual functions, where each subclass can provide its specific implementation for the virtual functions.

2. **Maintenance Challenges**: As the number of functions and subclasses grows, managing function pointers in a single class can become challenging. This can lead to decreased code modularity and maintainability.

3. **Design Constraints**: The function pointer approach requires that all subclasses adhere to the same interface, limiting the range of possibilities based on class types.

## ONNX Operator Types

ONNX defines various operator types to categorize different types of operations used in neural networks and machine learning models. These operator types help organize operations based on the number of input tensors they require and the type of computation they perform.

1. **Unary Operators**: Unary operators take a single input tensor and produce an output tensor. They perform element-wise computations on the input tensor. Examples include Abs, Neg, Exp, Log, and Sigmoid.

2. **Binary Operators**: Binary operators take two input tensors of the same shape and produce an output tensor. They perform element-wise computations on corresponding elements of the input tensors. Examples include Add, Sub, Mul, Div, and Pow.

3. **Math Operators**: Math operators include various mathematical operations used in neural networks and mathematical models. They cover arithmetic, trigonometric, and statistical functions, among others.

4. **NN (Neural Network) Operators**: NN operators are specialized operations used in neural network architectures. They include convolution, pooling, normalization, activation functions (ReLU, Tanh, etc.), and others commonly used in deep learning models.

5. **RNN (Recurrent Neural Network) Operators**: RNN operators are specific to recurrent neural networks and sequence processing. They include operations like RNN, LSTM, GRU, and others used for modeling sequential data.

6. **Reduction Operators**: Reduction operators reduce the size of the input tensor by aggregating elements along specified axes. Common reduction operators include Sum, Mean, Max, Min, and Prod.

7. **Gather and Scatter Operators**: These operators gather and scatter elements from one tensor to another based on specified indices. They are commonly used in data manipulations and indexing.

8. **Comparison Operators**: Comparison operators perform element-wise comparisons between two input tensors and produce a boolean output tensor. Examples include Greater, Less, Equal, and LogicalAnd.

These are just some examples of the operator types in ONNX. The ONNX specification continues to evolve, and new operator types may be introduced in later versions to support a broader range of operations in machine learning and deep learning models.

## Hybrid Approach for Handling Operators in ONNX

To handle the diverse set of unary and binary operators in ONNX efficiently, a hybrid approach is commonly used, combining polymorphism, inheritance, and function pointers. This approach provides flexibility and extensibility to support a long list of operators while keeping the codebase organized and maintainable.

1. **Polymorphism and Inheritance**: The hybrid approach starts by defining a common base class called `Operator`. This base class captures shared behavior and interfaces that are common to all operators. Each specific operator type, such as unary and binary operators, is derived from this base class, inheriting the shared behavior and implementing its specific functionality.

   ```cpp
   class Node {
   public:
       virtual ~Node() {}
       virtual void Activate() = 0; // Virtual function representing the core operation
       // Other shared functionality and data members can be included here
   };

   class UnaryOperator : public Node {
   public:
       void Activate() override ; // Implementation of the unary operation
   };
   typedef UnaryOperator * UnaryOperatorPtr;

   class BinaryOperator : public Node {
   public:
        void Activate() override ; // Implementation of the binary operation
   };
   typedef BinaryOperator * BinaryOperatorPtr;
   ```

2. **Function Pointers**: Instead of directly storing the actual implementation of each operator inside the classes, function pointers are used to point to the corresponding implementations. Function pointers provide flexibility and can be dynamically assigned to the appropriate function implementations during runtime.

   ```cpp
   typedef void (*UnaryFunctionPtr)(TensorPtr, TensorPtr, UnaryOperatorPtr);

   class UnaryOperator : public Node {
   public:
        UnaryOperator(const UnaryFunctionPtr typedFn[TDT_COUNT]):Node(){ this->_typedFn = _typedFn; }
        bool Activate() override ;

   protected:
        UnaryFunctionPtr * _typedFn;
   };

   ```

   with an UnaryOperator Activate implementation similar

   ```cpp
   bool  UnaryOperator::Activate()
   {
    ...
        TensorPtr input = &(a->Payload);
        int i = (int)input->Type;
        if( i >= 0 && i < TDT_COUNT )
        {
            UnaryFunctionPtr w = this->_typedFn[i];
            if(w)
            {
              (*w)(input, input, this);
            }
        }
    ...
        return true;
    }
    return false;
   }
   ```

3. **Usage**: With this hybrid approach, instances of specific operator types can be created, and at runtime, the appropriate function pointer is assigned based on the operator type. This allows for easy addition of new operators or modification of existing operators without changing the base class or overall structure.

   **_Operator Declaration_**

   ```cpp
   extern const UnaryFunctionPtr AbsFunctionArray[TDT_COUNT];

   class Abs: UnaryOperator {
    public: Abs() : UnaryOperator(AbsFunctionArray)
    {
    };
   };
   ```

   **_Operator Implementation_**

   ```cpp
   template <typename T>
    void AbsFunc (TensorPtr x, TensorPtr out, UnaryOperatorPtr node)
    {
        T* data = static_cast<T*>(x->Data);
        T* res = static_cast<T*>(out->Data);
        for (size_t i = 0; i < x->Count; ++i)
        {
            res[i] = ((a<0) ? -a : a) ;
        }
    };

    const UnaryFunctionPtr AbsFunctionArray[TDT_COUNT]=
    {
      nullptr,       // Placeholder for TDT_UNDEFINED
      &Abs<float>,   // Function for TDT_FLOAT
      nullptr,       // Function for TDT_UINT8
      &Abs<int8_t>,  // Function for TDT_INT8
      nullptr,       // Function for TDT_UINT16
      &Abs<int16_t>, // Function for TDT_INT16
      &Abs<int32_t>, // Function for TDT_INT32
      &Abs<int64_t>, // Function for TDT_INT64
      nullptr,       // Function for TDT_STRING
      nullptr,       // Function for TDT_BOOL
      nullptr,       // Function for TDT_FLOAT16
      nullptr,       // Function for TDT_DOUBLE
      nullptr,       // Function for TDT_UINT32
      nullptr,       // Function for TDT_UINT64
      nullptr,       // Function for TDT_COMPLEX64
      nullptr,       // Function for TDT_COMPLEX128
      nullptr,       // Function for TDT_BFLOAT16
      nullptr,       // Function for TDT_FLOAT8E4M3FN
      nullptr,       // Function for TDT_FLOAT8E4M3FNUZ
      nullptr,       // Function for TDT_FLOAT8E5M2
      nullptr        // Function for TDT_FLOAT8E5M2FNUZ
    };
   ```

   Furthermore, we can define MACRO to perform this declarations and implementations.

By adopting this hybrid approach, developers can effectively manage the various operators in ONNX, provide clear separation between operator types, and easily introduce new operators as needed, making the codebase more maintainable and adaptable.
