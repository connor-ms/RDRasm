#ifndef VECTOR_H
#define VECTOR_H

#include "../opcodefactory.h"

MAKE_SIMPLE_OP(Op_VAdd, EOpcodes::OP_VADD, "vadd", 1); // +
MAKE_SIMPLE_OP(Op_VSub, EOpcodes::OP_VSUB, "vsub", 1); // -
MAKE_SIMPLE_OP(Op_VMul, EOpcodes::OP_VMUL, "vmul", 1); // *
MAKE_SIMPLE_OP(Op_VDiv, EOpcodes::OP_VDIV, "vdiv", 1); // /
MAKE_SIMPLE_OP(Op_VNeg, EOpcodes::OP_VNEG, "vneg", 1); // ~

#endif // VECTOR_H
