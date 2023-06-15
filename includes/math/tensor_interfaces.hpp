#ifndef __CYAN_MISCELIUM_TENSOR_INTERFACES__
#define __CYAN_MISCELIUM_TENSOR_INTERFACES__

namespace CyanMycelium         
{
  class Shape
  {
    public:
       int * GetDimensions()
       {
        return this->_dimensions;
       } 
       
    private:
    int* _dimensions;
  };

  class Tensor 
  {
    public:
      Shape GetShape()
      {
        return this->_shape;
      }

    private:
      Shape _shape;
  };

}

#endif