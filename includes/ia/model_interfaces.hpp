#ifndef __CYAN_MISCELIUM_MODEL_INTERFACES__
#define __CYAN_MISCELIUM_MODEL_INTERFACES__

namespace CyanMycelium         
{
  class Shape ;

  class Model 
  {
    public:
      virtual Shape GetInputShape() = 0;
      virtual Shape GetOutputShape() = 0;
    private:
  };
}

#endif