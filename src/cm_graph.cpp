#include "cm_graph.hpp"

namespace CyanMycelium         
{
  bool  UnaryOperator::Activate() 
  {
    LinkPtr * l = this->Opsc;
    LinkPtr a = *l;
    if( a != nullptr ) {
        // this is the place we get the value holded by the link, which is the input tensor
        Tensor input = a->Payload;
        int i = (int)input.Type;
        if( i >= 0 && i < TDT_COUNT )
        {
            UnaryFunctionPtr w = this->_typedFn[i];
            if(w)
            {
              // TODO -> build a strategy about the resulting tensor
              (*w)(&input, &input, this);
            }
        }
        // transfert the input tensor content to the output tensor content.
        // assuming the tensors has the same shape.
        l = this->Onsc;
        a = *l;
        if( a != nullptr ) 
        {
            a ->Payload.Data = input.Data;
            input.Data = NULL; 
        }
        return true;
    }
    return false;
  }

  bool BinaryOperator::Activate() 
  {
    LinkPtr * l = this->Opsc;
    LinkPtr a = *l;
    LinkPtr b = *(l+1);

    if( a != nullptr  && b != nullptr ) 
    {
      Tensor tx = a->Payload;
      Tensor ty = b->Payload;
      if(tx.Type != ty.Type)
      {
        // Input type mismatch
        return false;
      }
      int i = (int)tx.Type;
      if( i >= 0 && i < TDT_COUNT )
      {
        BinaryFunctionPtr w = this->_typedFn[i];
        if(w)
        {
          // TODO -> build a strategy about the resulting tensor
          (*w)(&tx, &ty, &tx, this);
        }
      }
    }
    return false;
  }
} 