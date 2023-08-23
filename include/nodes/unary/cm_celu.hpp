#ifndef _CM_NODE_CELU__
#define _CM_NODE_CELU__
#include "cm_graph.hpp"

namespace CyanMycelium
{
#define CELU Celu

   extern const UnaryFunctionPtr CeluFunctionArray[TDT_COUNT];

   class Celu : public UnaryOperator
   {
   public:
      float Alpha;
      Celu() : UnaryOperator(CeluFunctionArray){};
      bool TrySetAtt(const char *n, Att_value_t v)
      {
         if (strcmp(n, "Alpha"))
         {
            Alpha = v.f;
            return true;
         }
         return false;
      }
   };
   typedef Celu *CeluPtr;

}

#endif