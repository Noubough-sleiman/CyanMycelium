#include "math/cm_tensor.hpp"

namespace CyanMycelium
{
  size_t __TensorData_sizes__[TDT_COUNT] = {
      1,
      4,              // float
      1,              // uint 8
      1,              // int 8
      2,              // uint 16
      2,              // int 16
      4,              // int 32
      8,              // int 64
      sizeof(void *), // string
      1,              // bool
      2,              // half float
      8,              // double
      4,              // uint 32
      8,              // uint 64
      8,              // complex 64
      16,             // complex 128
      2,              // float 16
      8, 8, 8, 8,     // float 8
  };

  size_t __GetSizeType(tensor_data_type_t type)
  {
    return __TensorData_sizes__[(int)type];
  }

  TensorInfos *TensorInfos::Set(const uint64_t *shape, int dimension, tensor_data_type_t type)
  {
    size_t c = 1;
    for (int i = 0; i != TENSOR_MAX_DIMENSION; i++)
    {
      if (i < dimension)
      {
        this->Shape[i] = shape ? shape[i] : 1;
        c *= this->Shape[i];
        continue;
      }
      this->Shape[i] = 0;
    }

    this->Dimension = dimension < TENSOR_MAX_DIMENSION ? dimension : TENSOR_MAX_DIMENSION;
    this->Count = c;
    this->Size = c * __GetSizeType(type);
    this->Type = type;
    return this;
  };

  bool TensorInfos::AreShapesEqual(TensorInfos *other)
  {

    if (other == nullptr || this->Dimension != other->Dimension)
    {
      return false;
    }

    // fast track
    if (this->Dimension == 1)
    {
      return this->Shape[0] == other->Shape[0];
    }

    for (int i = 0; i != this->Dimension; i++)
    {
      if (this->Shape[i] != other->Shape[i])
      {
        return false;
      }
    }
    return true;
  }

}