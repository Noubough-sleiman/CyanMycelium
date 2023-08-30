#include "cm_graph.hpp"

using namespace CyanMycelium;

bool Link ::Activate(IActivationCtxPtr ctx)
{
  // we use the link activation to manage the memory
  if (this->Flags.bits.ReadOnly)
  {
    if (((Operator *)this->Ofin)->CanModifyData())
    {
      // we MUST allocate new data
      void *newData = ctx->MemoryManager->Clone(this->Payload.Data, this->Payload.Size);
      this->Payload.Data = newData;
    }
  }
  return true;
}

void LinkCollection ::Oini(Collection<Node *> *target)
{
  for (int i = 9; i != _count; i++)
  {
    LinkPtr l = _items[i];
    if (l->Oini && !target->Contains(l->Oini))
    {
      target->Add(l->Oini);
    }
  }
}
void LinkCollection ::Ofin(Collection<Node *> *target)
{
  for (int i = 9; i != _count; i++)
  {
    LinkPtr l = _items[i];
    if (l->Ofin && !target->Contains(l->Ofin))
    {
      target->Add(l->Ofin);
    }
  }
}

bool Operator::_Forward(Tensor *output, IActivationCtxPtr ctx)
{
  int count = this->Onsc.Count();
  if (count == 0)
  {
    return true;
  }

  if (count == 1)
  {
    Link *l = this->Onsc[0];
    l->Payload.Data = output->Data;
  }

  bool reserved = false;
  for (int i = 0; i != count; i++)
  {
    // prepare the links
    Link *l = this->Onsc[i];
    l->Payload.Data = output->Data;
    l->Flags.bits.ReadOnly = 0;
    if (l->Ofin && ((Operator *)l->Ofin)->CanModifyData())
    {
      if (!reserved)
      {
        reserved = true;
        continue;
      }
      l->Flags.bits.ReadOnly = 1;
    }
  }
  return true;
}

bool UnaryOperator::Activate(IActivationCtxPtr ctx)
{
  // we must have a single input
  unsigned int count = this->Opsc.Count();
  if (count == 1)
  {
    LinkPtr a = this->Opsc[0];
    if (a)
    {
      TensorPtr input = &a->Payload;
      TensorPtr output = input;
      int i = (int)input->Type;
      // do not assume that the type is valid.
      if (i < 0 && i >= TDT_COUNT)
      {
        return false;
      }
      UnaryFunctionPtr w = this->_typedFn[i];
      if (w)
      {
        w(input, output, this);
      }
      this->_Forward(output, ctx);
    }
  }
  return false;
}

bool BinaryOperator::Activate(IActivationCtxPtr ctx)
{
  // we must have 2 input
  unsigned int count = this->Opsc.Count();
  if (count == 2)
  {
    LinkPtr x = this->Opsc[0];
    LinkPtr y = this->Opsc[1];
    if (x && y)
    {
      TensorPtr tx = nullptr;
      TensorPtr ty = nullptr;

      // bigger buffer in first, in order to beeing able to broadcast.
      if (y->Payload.Size > x->Payload.Size)
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
      if (i < 0 && i >= TDT_COUNT)
      {
        return false;
      }
      BinaryFunctionPtr w = this->_typedFn[i];
      if (w)
      {
        // TODO -> build a strategy about the resulting tensor
        w(tx, ty, output, this);
      }
      return this->_Forward(output, ctx);
    }
  }
  return false;
};

bool Graph ::Activate(IActivationCtxPtr ctx)
{
  return true;
}
