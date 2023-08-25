#ifndef _CM_GRAPH__
#define _CM_GRAPH__

#include "cm.h"
#include "concurrent/cm_concurrent.hpp"
#include "math/cm_tensor.hpp"
#include "memory/cm_memory_manager.hpp"
#include "nodes/cm_nodes_commons.hpp"
#include "collections/cm_collections.hpp"

namespace CyanMycelium
{
    // forward declaration
    class Node;
    class Link;
    class UnaryOperator;
    class BinaryOperator;
    class LinkCollection;
    class NodeCollection;

    typedef void (*UnaryFunctionPtr)(Tensor *, Tensor *, UnaryOperator *);
    typedef void (*BinaryFunctionPtr)(Tensor *, Tensor *, Tensor *, BinaryOperator *);

    /// @brief
    class IActivationCtx
    {
    public:
        IMemoryManagerPtr MemoryManager;

        virtual bool Activate(Node *) = 0;
        virtual bool Activate(Link *) = 0;

    protected:
        IActivationCtx(IMemoryManagerPtr mm)
        {
            MemoryManager = mm;
        }
    };
    typedef IActivationCtx *IActivationCtxPtr;

    enum ActivationEventType
    {
        CM_ACTIVATION_LINK,
        CM_ACTIVATION_NODE,
    };

    struct ActivationEvent
    {
        ActivationEventType Type;
        IActivationCtx *Context;
        void *Content;
    };

    /// @brief
    class GraphItem
    {
    };
    typedef GraphItem *GraphItemPtr;

    /// @brief
    class Link : public GraphItem
    {
    public:
        Link() : Link(nullptr, 1){};
        Link(const int64_t *shape, int dimension, tensor_data_type_t type = TDT_UNDEFINED) : Payload(shape, dimension, type)
        {
            Oini = nullptr;
            Ofin = nullptr;
        };
        Node *Oini;
        Node *Ofin;
        Tensor Payload;
        virtual bool Activate(IActivationCtxPtr ctx);
    };

    typedef Link *LinkPtr;

    /// @brief
    class LinkCollection : public Collection<LinkPtr>
    {
    public:
        LinkCollection(unsigned int initialCapacity = 2) : Collection<LinkPtr>(initialCapacity)
        {
        }
        void Oini(Collection<Node *> *target);
        void Ofin(Collection<Node *> *);
    };

    union Att_value_t
    {
        cm_float_t f;
        cm_int64_t i;
        TensorPtr t;
    };

    /// @brief
    class Node : public GraphItem
    {
    public:
        Node() : _lock()
        {
        }
        virtual ~Node() {}

        LinkCollection Opsc;
        LinkCollection Onsc;
        void Lock(int timeoutMillis = CM_INFINITE) { _lock.Take(timeoutMillis); }
        void Unlock() { _lock.Give(); }
        virtual bool Activate(IActivationCtxPtr ctx) = 0;
        // you may use attribute binding logic by name or index.
        virtual bool TrySetAtt(const char *n, Att_value_t value) { return true; }
        virtual bool TrySetAtt(int index, Att_value_t value) { return true; }

    private:
        Mutex _lock;
    };

    typedef Node *NodePtr;

    /// @brief
    class NodeCollection : public Collection<NodePtr>
    {
    public:
        NodeCollection(unsigned int initialCapacity = 2) : Collection<NodePtr>(initialCapacity)
        {
        }
    };

    /// @brief
    class Operator : public Node
    {
    protected:
        bool ForwardOuput(TensorPtr output, IActivationCtxPtr ctx);
    };
    typedef Operator *OperatorPtr;

    /// @brief
    class UnaryOperator : public Operator
    {
    public:
        UnaryOperator(const UnaryFunctionPtr typedFn[TDT_COUNT]) : Operator() { _typedFn = typedFn; }
        bool Activate(IActivationCtxPtr ctx) override;

    protected:
        const UnaryFunctionPtr *_typedFn;
    };
    typedef UnaryOperator *UnaryOperatorPtr;

    /// @brief
    class BinaryOperator : public Operator
    {
    public:
        BinaryOperator(const BinaryFunctionPtr typedFn[TDT_COUNT]) : Operator() { this->_typedFn = typedFn; }
        bool Activate(IActivationCtxPtr ctx) override;

    protected:
        const BinaryFunctionPtr *_typedFn;
    };
    typedef BinaryOperator *BinaryOperatorPtr;

#ifndef CM_DEFAULT_GRAPH_COLLECTION_CAPACITY
#define CM_DEFAULT_GRAPH_COLLECTION_CAPACITY 16
#endif

    /// @brief The Runtime support for the computational graph.
    /// As node we use  Object Positive Semi Conductor (Opsc) as input
    /// and Object Negativ Semi Conductor (Onsc) as output
    /// please not this is a Runtime support and all the necessay initialization has to be done by
    /// a corresponding GraphBuilder
    class Graph : public Node
    {
    public:
        Graph(int initialNodesCollectionSize = CM_DEFAULT_GRAPH_COLLECTION_CAPACITY, int initialLinkCollectionSize = CM_DEFAULT_GRAPH_COLLECTION_CAPACITY) : Nodes(max(initialNodesCollectionSize, CM_DEFAULT_COLLECTION_CAPACITY)),
                                                                                                                                                             Links(max(initialLinkCollectionSize, CM_DEFAULT_COLLECTION_CAPACITY))
        {
        }
        ~Graph() {}
        /// @brief NULL terminated list of nodes.
        NodeCollection Nodes;
        /// @brief NULL terminated list of links.
        LinkCollection Links;

        KeyValueCollection<LinkPtr> inputs;
        KeyValueCollection<LinkPtr> outputs;

        bool Activate(IActivationCtxPtr ctx) override;
    };
    typedef Graph *GraphPtr;
}
#endif