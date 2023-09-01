#include "cm_graph.hpp"

using namespace CyanMycelium;

bool Link ::Activate(ActivationContext *ctx)
{
  return true;
}
bool UnaryOperator::Activate(ActivationContext *ctx)
{
  // we must have a single input
  unsigned int count = this->Opsc.Count();
  if (count == 1)
  {
    Link *a = this->Opsc[0];
    if (a)
    {
      TensorRef *input = ctx->GetPayloadRef(a->Id);
      TensorRef *output = input;
      int i = (int)input->Value.Type;
      // do not assume that the type is valid.
      if (i < 0 && i >= TDT_COUNT)
      {
        return false;
      }
      UnaryFunctionPtr w = this->_typedFn[i];
      if (w)
      {
        w(&input->Value, &output->Value, this);
      }
      return ctx->Forward(this, output);
    }
  }
  return false;
}

bool BinaryOperator::Activate(ActivationContext *ctx)
{
  // we must have 2 input
  unsigned int count = this->Opsc.Count();
  if (count == 2)
  {
    Link *x = this->Opsc[0];
    Link *y = this->Opsc[1];
    if (x && y)
    {
      TensorRef *refx = ctx->GetPayloadRef(x->Id);
      TensorRef *refy = ctx->GetPayloadRef(y->Id);
      if (!refy)
      {
        return false;
      }
      // bigger buffer in first, in order to beeing able to broadcast.
      if (refy->Value.Size > refx->Value.Size)
      {
        TensorRef *tmp = refx;
        refx = refy;
        refy = tmp;
      }

      TensorRef *output = refx;

      int i = (int)refx->Value.Type;
      // do not assume that the type is valid.
      if (i < 0 && i >= TDT_COUNT)
      {
        return false;
      }
      BinaryFunctionPtr w = this->_typedFn[i];
      if (w)
      {
        // TODO -> build a strategy about the resulting tensor
        w(&refx->Value, &refy->Value, &output->Value, this);
      }
      return ctx->Forward(this, output);
    }
  }
  return false;
};

bool Graph ::Activate(ActivationContext *ctx)
{
  return true;
}
