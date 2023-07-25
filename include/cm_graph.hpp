#ifndef __CYAN_MISCELIUM_GRAPH__
#define __CYAN_MISCELIUM_GRAPH__

#include "cm.h"
#include "math/cm_tensor.hpp"
#include "nodes/cm_nodes_commons.hpp"

namespace CyanMycelium
{

    class GraphItem
    {
    };

    class Link : GraphItem 
    {
        public:
        Node * Oini;
        Node * Ofin;
        Tensor Payload;
    };
    typedef Link * LinkPtr ;

    class Node : GraphItem 
    {
        public:
        LinkPtr * Opsc;
        LinkPtr * Onsc;
        virtual bool Activate() = 0 ;

    };

    typedef Node * NodePtr ;

    class UnaryOperator : Node
    {
        public:
        UnaryOperator(const UnaryFunctionPtr typedFn[TDT_COUNT]):Node(){ this->_typedFn = _typedFn; }
        bool Activate() override ;

        protected:
        UnaryFunctionPtr * _typedFn;
    };
    typedef UnaryOperator * UnaryOperatorPtr;

    class BinaryOperator : Node
    {
        public:
        BinaryOperator(const BinaryFunctionPtr typedFn[TDT_COUNT]):Node(){ this->_typedFn = _typedFn; }
        bool Activate()  override  ;

        protected:
        BinaryFunctionPtr * _typedFn;
    };

    typedef BinaryOperator * BinaryOperatorPtr;
    
    class Graph : Node {
    };
}

#endif