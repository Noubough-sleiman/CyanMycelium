#ifndef __CYAN_MISCELIUM_GRAPH__
#define __CYAN_MISCELIUM_GRAPH__

#include "cm.h"
#include "math/cm_tensor.hpp"
#include "nodes/cm_nodes_commons.hpp"

namespace CyanMycelium
{

    /// @brief 
    class GraphItem
    {
    };

    /// @brief 
    class Link : public GraphItem 
    {
        public:
        Node * Oini;
        Node * Ofin;
        Tensor Payload;
    };
    typedef Link * LinkPtr ;

    /// @brief 
    class Node : public GraphItem 
    {
        public:
        LinkPtr * Opsc;
        LinkPtr * Onsc;
    };

    typedef Node * NodePtr ;

    /// @brief 
    class Operator : public Node {
        public:
        virtual bool Activate() = 0 ;
    } ;
    typedef Operator * OperatorPtr;

    /// @brief 
    class UnaryOperator : public Operator
    {
        public:
        UnaryOperator(const UnaryFunctionPtr typedFn[TDT_COUNT]):Operator(){ this->_typedFn = _typedFn; }
        bool Activate() override ;

        protected:
        UnaryFunctionPtr * _typedFn;
    };

    typedef UnaryOperator * UnaryOperatorPtr;

    /// @brief 
    class BinaryOperator : public Operator
    {
        public:
        BinaryOperator(const BinaryFunctionPtr typedFn[TDT_COUNT]):Operator(){ this->_typedFn = _typedFn; }
        bool Activate()  override  ;

        protected:
        BinaryFunctionPtr * _typedFn;
    };

    typedef BinaryOperator * BinaryOperatorPtr;
    
    /// @brief 
    class Graph : Node {
        /// @brief NULL terminated list of nodes.
        NodePtr * Nodes;
        /// @brief NULL terminated list of links.
        LinkPtr * Links;
    };
}

#endif