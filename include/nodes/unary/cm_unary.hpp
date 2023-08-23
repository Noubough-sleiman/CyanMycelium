#ifndef _CM_NODE_UNARY__
#define _CM_NODE_UNARY__
#include "cm_graph.hpp"

namespace CyanMycelium
{
#define ABS Abs
#define ACOS Acos
#define ACOSH Acosh
#define ASIN Asin
#define ASINH Asinh
#define ATAN Atan
#define ATANH Atanh
#define CEIL Ceil
#define CELU Celu
#define COS Cos
#define COSH Cosh
#define ELU Elu
#define EXP Exp
#define FLOOR Floor

   UNARY_OP_DECL(ABS)
   UNARY_OP_DECL(ACOS)
   UNARY_OP_DECL(ACOSH)
   UNARY_OP_DECL(ASIN)
   UNARY_OP_DECL(ASINH)
   UNARY_OP_DECL(ATAN)
   UNARY_OP_DECL(ATANH)
   UNARY_OP_DECL(CEIL)
   UNARY_OP_DECL(COS)
   UNARY_OP_DECL(COSH)
   UNARY_OP_DECL(ELU)
   UNARY_OP_DECL(EXP)
   UNARY_OP_DECL(FLOOR)
}

#endif