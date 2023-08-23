#ifndef _CM_NODE_BINARY__
#define _CM_NODE_BINARY__
#include "cm_graph.hpp"

namespace CyanMycelium
{
#define ADD Add
#define AND And
#define DIV Div
#define EQUAL Equal
#define GREATER Greater
#define LESS Less
#define MAX Max
#define MIN Min
#define MULT Mult
#define OR Or
#define POW Pow
#define SUB Sub
#define WHERE Where
#define XOR Xor

  BINARY_OP_DECL(ADD)
  BINARY_OP_DECL(AND)
  BINARY_OP_DECL(DIV)
  BINARY_OP_DECL(EQUAL)
  BINARY_OP_DECL(GREATER)
  BINARY_OP_DECL(LESS)
  BINARY_OP_DECL(MAX)
  BINARY_OP_DECL(MIN)
  BINARY_OP_DECL(MULT)
  BINARY_OP_DECL(OR)
  BINARY_OP_DECL(POW)
  BINARY_OP_DECL(SUB)
  BINARY_OP_DECL(WHERE)
  BINARY_OP_DECL(XOR)
}
#endif