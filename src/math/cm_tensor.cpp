#include "math/cm_tensor.hpp"

namespace CyanMycelium         
{
  Tensor::Tensor( ) 
  {
    Set(nullptr,1);
  };

  Tensor::Tensor(const uint32_t * shape, int dimension ) 
  {
    Set(shape,dimension);
  };

  TensorPtr Tensor::Set(const uint32_t * shape, int dimension, tensor_data_type_t type) 
  {
      uint8_t d = dimension < TENSOR_MAX_DIMENSION ? dimension : TENSOR_MAX_DIMENSION;
      size_t c = 1;
      for(int i=0; i != d; i++)
      {
        Shape[i] = shape ? shape[i] : 1;
        c *= Shape[i];
      }
      Dimension = d;
      Count = c;
      Size = 0;
      Data = nullptr;
      Type = type;
      return this;
  };

  bool Tensor::ShapesAreEqual(TensorPtr other){
    
    if(other == nullptr || this->Dimension != other->Dimension)
    {
      return false;
    }

    // fast track
    if( this->Dimension == 1)
    {
      return this->Shape[0] == other->Shape[0];
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