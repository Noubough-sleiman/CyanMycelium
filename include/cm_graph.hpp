#ifndef __CYAN_MISCELIUM_GRAPH__
#define __CYAN_MISCELIUM_GRAPH__

#include "cm.h"
#include "concurrent/cm_concurrent.hpp"
#include "math/cm_tensor.hpp"
#include "memory/cm_memory_manager.hpp"
#include "nodes/cm_nodes_commons.hpp"
#include "cm_graph_utils.hpp"

namespace CyanMycelium
{
    typedef Link * LinkPtr ;
    typedef Node * NodePtr ;

    /// @brief 
    class IActivationCtx
    {
        public:
        IMemoryManagerPtr MemoryManager;

        virtual bool Activate(LinkPtr, void *) = 0;
        virtual bool Activate(NodePtr) = 0;
        
        protected:
        IActivationCtx(IMemoryManagerPtr mm)
        {
            MemoryManager = mm;
        }
    };
    typedef IActivationCtx * IActivationCtxPtr;

    /// @brief 
    class GraphItem
    {
    };
    typedef GraphItem * GraphItemPtr ;

    /// @brief 
    class Link : public GraphItem 
    {
        public:
        NodePtr Oini;
        NodePtr Ofin;
        Tensor Payload;
        virtual bool Activate(uint8_t * input, IActivationCtxPtr ctx);
    };

    /// @brief 
    class LinkCollection : public Collection<LinkPtr>
    {
    };

    /// @brief 
    class Node : public GraphItem 
    {
        public:
        Node();
        ~Node();
        LinkCollection Opsc;
        LinkCollection Onsc;
        MutexPtr GetLock();

        virtual bool Activate(IActivationCtxPtr ctx) = 0 ;   
 
        private :
        MutexPtr _lock; 
    };

    /// @brief 
    class NodeCollection : public Collection<NodePtr>
    {
    };

    /// @brief 
    class Operator : public Node {
        protected :
        bool ForwardOuput(TensorPtr output, IActivationCtxPtr ctx) ;
    } ;
    typedef Operator * OperatorPtr;

    /// @brief 
    class UnaryOperator : public Operator
    {
        public:
        UnaryOperator(const UnaryFunctionPtr typedFn[TDT_COUNT]):Operator(){ this->_typedFn = _typedFn; }
        bool Activate(IActivationCtxPtr ctx) override ;

        protected:
        UnaryFunctionPtr * _typedFn;
    };
    typedef UnaryOperator * UnaryOperatorPtr;

    /// @brief 
    class BinaryOperator : public Operator
    {
        public:
        BinaryOperator(const BinaryFunctionPtr typedFn[TDT_COUNT]):Operator(){ this->_typedFn = _typedFn; }
        bool Activate(IActivationCtxPtr ctx)  override  ;

        protected:
        BinaryFunctionPtr * _typedFn;
    };
    typedef BinaryOperator * BinaryOperatorPtr;
    
    /// @brief The Runtime support for the computational graph. 
    /// As node we use  Object Positive Semi Conductor (Opsc) as input 
    /// and Object Negativ Semi Conductor (Onsc) as output
    /// please not this is a Runtime support and all the necessay initialization has to be done by
    /// a corresponding GraphBuilder 
    class Graph : public Node {
        public :
        /// @brief NULL terminated list of nodes.
        NodeCollection Nodes;
        /// @brief NULL terminated list of links.
        LinkCollection Links;

        KeyValueCollection<LinkPtr> inputs;
        KeyValueCollection<LinkPtr> outputs;
    };
    typedef Graph *  GraphPtr;

    /// @brief 
    class IGraphBuilder {
        public:
        virtual GraphPtr Build(...) = 0;
    };

    typedef IGraphBuilder * IGraphBuilderPtr;
}
#endif