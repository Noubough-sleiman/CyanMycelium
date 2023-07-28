#include "cm_graph.hpp"

using  namespace CyanMycelium ;         

  bool Link :: Activate(uint8_t * input, IActivationCtxPtr ctx)
  {
     this-> Payload.Data = input;
  }  

  bool Operator:: ForwardOuput(TensorPtr output, IActivationCtxPtr ctx)
  {
      switch(this->Onsc.Count)
      {
        case 0 : return true;
        case 1 : return ctx->Activate(this->Onsc[0], output->Data);
        default:
        {
          if( ctx->Activate(this->Onsc[0], output->Data) == false)
          {
            return false;
          }
            
          for(int i=1; i < this->Onsc.Count ; ++i)
          {
            void * buffer = ctx->MemoryManager->Clone(output->Data, output->Size);
            if( !buffer || ! ctx->Activate(this->Onsc[0], buffer))
            {
              return false;
            }
          }
        }
      }
      return true;
  }

  bool  UnaryOperator::Activate(IActivationCtxPtr ctx) 
  {
    // we must have a single input
    if( this->Opsc.Count == 1)
    {
      LinkPtr a = this->Opsc[0];
      if( a )
      {
        TensorPtr input = &a->Payload;
        TensorPtr output = input;
        int i = (int)input->Type;
        // do not assume that the type is valid.
        if( i < 0 && i >= TDT_COUNT )
        {
          return false;
        }
        UnaryFunctionPtr w = this->_typedFn[i];
        if(w)
        {
          (*w)(input, output, this);
        }
        return this->ForwardOuput(output,ctx);
      }
    }
    return false;
  }

  bool BinaryOperator::Activate(IActivationCtxPtr ctx) 
  {
    // we must have 2 input
    if( this->Opsc.Count == 2)
    {
      LinkPtr x = this->Opsc[0];
      LinkPtr y = this->Opsc[1];
      if( x && y )
      {
        TensorPtr tx = nullptr;
        TensorPtr ty = nullptr;

        // bigger buffer in first, in order to beeing able to broadcast. 
        if( y->Payload.Size > y->Payload.Size)
        {
            tx = &y->Payload;
            ty = &x->Payload;
        } 
        else 
        {
            tx = &x->Payload;
            ty = &y->Payload;
        }
        TensorPtr output = tx;

        int i = (int)tx->Type;
        // do not assume that the type is valid.
        if( i < 0 && i >= TDT_COUNT )
        {
          return false;
        }
        BinaryFunctionPtr w = this->_typedFn[i];
        if(w)
        {
          // TODO -> build a strategy about the resulting tensor
          (*w)(tx, ty, output, this);
        }
        
        // free the smaller buffer.
        ctx->MemoryManager->Free(ty->Data);
        ty->Data = nullptr;

        return this->ForwardOuput(output,ctx);
      }
    }
    return false;
}; 