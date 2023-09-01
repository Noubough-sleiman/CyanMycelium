#ifndef _CM_GRAPH_
#define _CM_GRAPH_

#include "cm.h"

#include "cm_activation.hpp"
#include "nodes/cm_nodes_commons.hpp"
#include "collections/cm_collections.hpp"

namespace CyanMycelium
{
    // forward declaration
    class Node;
    class Operator;
    class Link;
    class UnaryOperator;
    class BinaryOperator;
    class LinkCollection;

    /// @brief The base class for all graph items.
    class GraphItem
    {
    public:
        // optional Identifier.
        int32_t Id = -1;
    };
    typedef GraphItem *GraphItemPtr;

    /// @brief The base class for all links.
    class Link : public GraphItem
    {
    public:
        Link() : Link(nullptr, 1){};
        Link(const uint64_t *shape, int dimension, tensor_data_type_t type = TDT_UNDEFINED) : _payloadInfos(shape, dimension, type)
        {
            Oini = nullptr;
            Ofin = nullptr;
        };

        /// @brief Activate the link.
        /// @param ctx the activation context
        /// @return  true if the activation is successful and false otherwise.
        virtual bool Activate(ActivationContext *ctx);

        /// @brief  Get the payload infos of the link. This is the Tensor info hol by the link itself.
        /// @return  the payload infos.
        virtual Tensor *GetPayloadInfos() { return &this->_payloadInfos; }

        /// @brief Set the payload infos of the link.
        /// @param shape the shape as an array of the size of each dimension
        /// @param dimension the number of axes or indices required to access the elements of the tensor.
        /// @param type the type of the underlying elements
        /// @param data the data to be set. Default is null.
        virtual void SetPayloadInfos(const uint64_t *shape, int dimension, tensor_data_type_t type, void *data = nullptr) { this->_payloadInfos.Set(shape, dimension, type, data); }

        /// @brief the operator that is the source of the link. May be null for input links.
        Operator *Oini;
        /// @brief the operator that is the destination of the link. May be null for output links.
        Operator *Ofin;

    protected:
        /// @brief the tensor is holding the infos of the data.
        /// Additionaly, to support Initializer, we keep reference to a full tensor with data.
        /// These informations are NOT intended to be mutable.
        Tensor _payloadInfos;
    };

    union Att_value_t
    {
        cm_float_t f;
        cm_int64_t i;
        TensorPtr t;
    };

    /// @brief The base class for all nodes.
    class Node : public GraphItem
    {
    public:
        Node() : _lock()
        {
        }
        virtual ~Node() {}

        /// @brief List of Incoming links.
        Collection<Link *> Opsc;

        /// @brief List of Outgoing links.
        Collection<Link *> Onsc;

        /// @brief Lock the node.
        /// @param timeoutMillis
        void Lock(int timeoutMillis = CM_INFINITE) { _lock.Take(timeoutMillis); }

        /// @brief Unlock the node.
        void Unlock() { _lock.Give(); }

        /// @brief Used to bind an attribute by name.
        /// @param n the name of the attribute
        /// @param value the value of the attribute
        /// @return true if the attribute is bound and false otherwise.
        virtual bool TrySetAtt(const char *n, Att_value_t value) { return true; }

        /// @brief Used to bind an attribute by index.
        /// @param i the index of the attribute
        /// @param value the value of the attribute
        /// @return true if the attribute is bound and false otherwise.
        virtual bool TrySetAtt(int i, Att_value_t value) { return true; }

    private:
        /// @brief the lock used to protect the node.
        Mutex _lock;
    };

    typedef Node *NodePtr;

    /// @brief The base class for all operators.
    class Operator : public Node
    {
    public:
        /// @brief the activation function
        /// @param ctx  the activation context
        /// @return true if the activation is successful and false otherwise.
        virtual bool Activate(ActivationContext *ctx) = 0;

        /// @brief return true if the operator is mutable, which mean it will change the input data
        /// @return true if the operator is mutable and false otherwise.
        virtual bool IsMutable() { return true; }

    protected:
        /// @brief Push the outgoing result to the next operator by settting the payload of the outgoing link.
        /// @param output the outgoing link
        /// @param ctx the activation context
        /// @return true if the activation is successful and false otherwise.
        bool _forward(Tensor *output, ActivationContext *ctx);
    };

    typedef Operator *OperatorPtr;

    typedef void (*UnaryFunctionPtr)(Tensor *, Tensor *, UnaryOperator *);

    /// @brief
    class UnaryOperator : public Operator
    {
    public:
        UnaryOperator(const UnaryFunctionPtr typedFn[TDT_COUNT]) : Operator() { _typedFn = typedFn; }
        bool Activate(ActivationContext *ctx) override;

    protected:
        const UnaryFunctionPtr *_typedFn;
    };
    typedef UnaryOperator *UnaryOperatorPtr;

    typedef void (*BinaryFunctionPtr)(Tensor *, Tensor *, Tensor *, BinaryOperator *);

    /// @brief
    class BinaryOperator : public Operator
    {
    public:
        BinaryOperator(const BinaryFunctionPtr typedFn[TDT_COUNT]) : Operator() { this->_typedFn = typedFn; }
        bool Activate(ActivationContext *ctx) override;

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
    class Graph : public Operator
    {
    public:
        Graph(int initialNodesCollectionSize = CM_DEFAULT_GRAPH_COLLECTION_CAPACITY, int initialLinkCollectionSize = CM_DEFAULT_GRAPH_COLLECTION_CAPACITY) : Nodes(max(initialNodesCollectionSize, CM_DEFAULT_COLLECTION_CAPACITY)),
                                                                                                                                                             Links(max(initialLinkCollectionSize, CM_DEFAULT_COLLECTION_CAPACITY))
        {
        }
        ~Graph() {}

        /// @brief list of nodes.
        Collection<Operator *> Nodes;
        /// @brief list of links.
        Collection<Link *> Links;

        KeyValueCollection<Link *> Inputs;
        KeyValueCollection<Link *> Outputs;

        bool Activate(ActivationContext *ctx) override;
    };
    typedef Graph *GraphPtr;
}
#endif