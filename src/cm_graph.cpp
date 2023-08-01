#include "cm_graph.hpp"

using  namespace CyanMycelium ;         

const char CM_DEFAULT_NAME[] = {'d','e','f','a','u','l', 't'};   

  Node :: Node()
  {
     _lock = new Mutex();
  }

  Node :: ~Node()
  {
      delete(_lock);
  }

  MutexPtr Node :: GetLock()
  {
    return _lock;
  }


  bool Link :: Activate(uint8_t * input, IActivationCtxPtr ctx)
  {
     this-> Payload.Data = input;
     return true;
  }  

  bool Graph :: Activate(IActivationCtxPtr ctx)
  {
    return true;
  }

  bool Operator:: ForwardOuput(TensorPtr output, IActivationCtxPtr ctx)
  {
      unsigned int count = this->Onsc.Count();
      switch(count)
      {
        case 0 : return true;
        case 1 : return ctx->Activate(this->Onsc[0], output->Data);
        default:
        {
          if( ctx->Activate(this->Onsc[0], output->Data) == false)
          {
            return false;
          }
            
          for(unsigned int i=1; i < count ; ++i)
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
    unsigned int count = this->Opsc.Count();

    if( count == 1)
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
    unsigned int count = this->Opsc.Count();
    if( count == 2)
    {
      LinkPtr x = this->Opsc[0];
      LinkPtr y = this->Opsc[1];
      if( x && y )
      {
        TensorPtr tx = nullptr;
        TensorPtr ty = nullptr;

        // bigger buffer in first, in order to beeing able to broadcast. 
        if( y->Payload.Size > x->Payload.Size)
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