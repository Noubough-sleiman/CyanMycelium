#ifndef __CYAN_MISCELIUM_ENGINE_INTERFACES__
#define __CYAN_MISCELIUM_ENGINE_INTERFACES__

namespace CyanMycelium         
{
  class Model;
  class Tensor;

  class Engine 
  {
    public:
        virtual int runInference(Model * model, Tensor * input, Tensor * output) = 0;
    private:
  };
}

#endif