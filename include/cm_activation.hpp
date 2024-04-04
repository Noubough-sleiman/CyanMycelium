#ifndef _CM_ACTIVATION_
#define _CM_ACTIVATION_

#include "memory/cm_memory_manager.hpp"
#include "math/cm_tensor.hpp"
#include "concurrent/cm_concurrent.hpp"

namespace CyanMycelium
{
    // forward declaration
    class Operator;
    class Link;
    class Graph;

    /// @brief TensorRef is the tensor reference used by the ActivationContext. This is a key element of the inference session.
    class TensorRef
    {
    public:
        TensorRef() : TensorRef(nullptr, 1)
        {
        }
        TensorRef(TensorRef &t) : TensorRef(t.Value)
        {
        }
        TensorRef(Tensor &t) : TensorRef(t.Shape, t.Dimension, t.Type)
        {
        }
        TensorRef(const uint64_t *shape, int dimension, tensor_data_type_t type = TDT_UNDEFINED) : Value(shape, dimension, type)
        {
            Count = 0;
            Flags.Value = 0;
        }
        Tensor Value; // the tensor value
        int Count;    // the number of links using this tensor
        union
        {
            struct
            {
                unsigned char ReadOnly : 1; // the tensor is NOT mutable.
                unsigned char Internal : 1; // the tensor is belong to the context.
                unsigned char Reserved : 6; // reserved
            } Bits;
            unsigned char Value;
        } Flags;

        /// @brief Lock the tensor reference. Locking the tensor reference is mandatory to modify the tensor value, flags and the reference count.
        /// @param timeoutMillis the timeout in milliseconds, default is CM_INFINITE
        void Lock(int timeoutMillis = CM_INFINITE) { _lock.Take(timeoutMillis); }

        /// @brief Unlock the tensor reference.
        void Unlock() { _lock.Give(); }

    private:
        /// @brief the lock object as Mutex.
        Mutex _lock;
    };

    typedef TensorRef *TensorRefPtr;

    /// @brief LinkState is the current state of a link. It holds the tensor reference and the link flags.
    class LinkState
    {
    public:
        LinkState() : Ref(nullptr)
        {
            Flags.Value = 0;
        };

        TensorRef *Ref;

        /// @brief the flags of the link.
        /// A link is either activ or inactiv. Activ link means it ready to be processed by the next Operator.
        union
        {
            struct
            {
                unsigned char Activ : 1;    // The link is activ, which mean it hold an input waiting to be processed.
                unsigned char Reserved : 7; // reserved
            } Bits;
            unsigned char Value;
        } Flags;
    };

    /// @brief ActivationContext is the context of the inference session. Each time we analyze an input, the inference session is supported with
    /// an ActivationContext. The ActivationContext is responsible to hold the tensor references, values and the memory manager.
    /// The ActivationContext is also responsible to activate and deactivate the links and operators.
    /// Thanks to this approach, the Graph is totally stateless and is only the suppport for the topology.
    class ActivationContext
    {
    public:
        /// @brief Construct a new Activation Context object
        /// @param model  the topology
        /// @param mm  the memory manager
        /// @return  the new ActivationContext object
        ActivationContext(Graph *model, IMemoryManagerPtr mm)
        {
            this->_model = model;
            this->_mm = mm;
            _buildTensorRefs();
        }

        /// @brief Destroy the Activation Context object
        virtual ~ActivationContext() { _clearTensorRefs(); }

        /// @brief Get the Memory Manager object
        /// @return the memory manager
        IMemoryManagerPtr GetMemoryManager() { return this->_mm; }

        /// @brief Get the Model object
        // @return the topology
        Graph *GetModel() { return _model; }

        void BindInput(const char *name, void *buffer);

        void BindOutput(const char *name, void *buffer);

        /// @brief Run the inference with the given input tensors.
        /// The input tensors are supposed to be binded previously with the input links.
        /// @return true if the operation is successful, false otherwise.
        bool Run();

        /// @brief Activate the operator. Operator activation means to gather data from input links, then process the data within the operator logic.
        /// Additionally, the operator activation will forward the output tensor to the next operator and deactivate the inputs links.
        /// This last operation is done with the support of the ForwardOutput method
        /// @param op  the operator
        /// @return true if the operation is successful, false otherwise.
        virtual bool Activate(Operator *);

        /// @brief Activate the link. Link activation means to verify that the link and its eventuals siblings are activ and to activate the next operator.
        /// @param l  the link
        /// @return true if the operation is successful, false otherwise.
        virtual bool Activate(Link *, TensorRefPtr = nullptr);

        /// @brief Deactivate the operator.
        /// @param op  the operator
        /// @return true if the operation is successful, false otherwise.
        virtual bool Deactivate(Operator *);

        /// @brief Deactivate the link.
        /// @param l  the link
        /// @return true if the operation is successful, false otherwise.
        virtual bool Deactivate(Link *);

        /// @brief Get the Payload Ref object
        /// @param id  the id of the link
        /// @return the tensor reference pointer
        virtual TensorRefPtr GetPayloadRef(int id) { return this->_states[id].Ref; }

        /// @brief Clone a tensor reference. This is usefull when the tensor need to be shared by multiple mutable node or branches
        /// TensorRef cloning is done with the support of the memory manager for the data copy.
        /// The new TensorRef is owned by the ActivationContext and consequently its Flags.Internal is set to 1.
        /// @param t the tensor reference to clone
        /// @return the new tensor reference
        virtual TensorRefPtr CloneRef(TensorRef &);

        /// @brief Forward the operator's result to the next operator.
        //  The context is concurrent, trying to hold the tensor ownership to avoid memory waste.
        /// @param outputValue  the output tensor
        /// @param op  the operator
        /// @return true if the operation is successful. false otherwise.
        virtual bool Forward(Operator *op, TensorRefPtr outputValue);

        /// @brief Forward the link's value to the next operator.
        /// @param l  the link
        /// @return true if the operation is successful, false otherwise.
        virtual bool Forward(Link *l);

        /// @brief Called when the outputs are ready.
        virtual bool OnOutputReady() { return true; }

    private:
        IMemoryManagerPtr _mm; // the memory manager
        Graph *_model;         // the model
        LinkState *_states;    // tensor references

        /// @brief Build the tensor references at construct time
        virtual void _buildTensorRefs();

        /// @brief Clear the tensor references at destruct time
        virtual void _clearTensorRefs();

        void _bind(Link *link, void *buffer);
    };
}
#endif