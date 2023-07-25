#include "./math/cm_tensor.hpp"

namespace CyanMycelium         
{
  Tensor::Tensor(uint32_t * shape, int dimension ) 
  {
      uint8_t d = dimension < TENSOR_MAX_DIMENSION ? dimension : TENSOR_MAX_DIMENSION;
      size_t c = 1;
      for(int i=0; i != d; i++)
      {
        this->Shape[i] = shape[i];
        c *= this->Shape[i];
      }
      this->Dimension = d;
      this->Count = c;
      this->Size = 0;
      this->Data = nullptr;
      this->Type = TDT_UNDEFINED;
  };

  bool Tensor::ShapesAreEqual(TensorPtr other){
    
    if(other == nullptr || this->Dimension != other->Dimension)
    {
      return false;
    }

    // fast track
    if( this->Dimension == 1)
    {
      return this->Shape[0] == other->Shape[0]
    }

    for(int i=0; i != this->Dimension; i++)
    {
      if(this->Shape[i] != other->Shape[i])
      {
        return false;
      }
    }
    return true;
  }

}