#ifndef FLOAT_H
#define FLOAT_H

#include "../opcodefactory.h"

// Float stack operations, takes most recent stack additions
MAKE_SIMPLE_OP(Op_FAdd,   EOpcodes::OP_FADD,   "fadd",   1); // +
MAKE_SIMPLE_OP(Op_FSub,   EOpcodes::OP_FSUB,   "fsub",   1); // -
MAKE_SIMPLE_OP(Op_FMul,   EOpcodes::OP_FMUL,   "fmul",   1); // *
MAKE_SIMPLE_OP(Op_FDiv,   EOpcodes::OP_FDIV,   "fdiv",   1); // /
MAKE_SIMPLE_OP(Op_FMod,   EOpcodes::OP_FMOD,   "fmod",   1); // %
MAKE_SIMPLE_OP(Op_FNeg,   EOpcodes::OP_FNEG,   "fneg",   1); // ~
MAKE_SIMPLE_OP(Op_FCmpEq, EOpcodes::OP_FCMPEQ, "fcmpeq", 1); // ==
MAKE_SIMPLE_OP(Op_FCmpNe, EOpcodes::OP_FCMPNE, "fcmpne", 1); // !=
MAKE_SIMPLE_OP(Op_FCmpGt, EOpcodes::OP_FCMPGT, "fcmpgt", 1); // >
MAKE_SIMPLE_OP(Op_FCmpGe, EOpcodes::OP_FCMPGE, "fcmpge", 1); // >=
MAKE_SIMPLE_OP(Op_FCmpLt, EOpcodes::OP_FCMPLT, "fcmplt", 1); // <
MAKE_SIMPLE_OP(Op_FCmpLe, EOpcodes::OP_FCMPLE, "fcmple", 1); // <=

MAKE_SIMPLE_OP(Op_FToI, EOpcodes::OP_FTOI, "ftoi", 1); // Float to int

// Push float val to stack
MAKE_SIMPLE_OP(Op_FPushN1, EOpcodes::OP_FPUSHN1, "fpushn1", 1);
MAKE_SIMPLE_OP(Op_FPush0,  EOpcodes::OP_FPUSH0,  "fpush0",  1);
MAKE_SIMPLE_OP(Op_FPush1,  EOpcodes::OP_FPUSH1,  "fpush1",  1);
MAKE_SIMPLE_OP(Op_FPush2,  EOpcodes::OP_FPUSH2,  "fpush2",  1);
MAKE_SIMPLE_OP(Op_FPush3,  EOpcodes::OP_FPUSH3,  "fpush3",  1);
MAKE_SIMPLE_OP(Op_FPush4,  EOpcodes::OP_FPUSH4,  "fpush4",  1);
MAKE_SIMPLE_OP(Op_FPush5,  EOpcodes::OP_FPUSH5,  "fpush5",  1);
MAKE_SIMPLE_OP(Op_FPush6,  EOpcodes::OP_FPUSH6,  "fpush6",  1);
MAKE_SIMPLE_OP(Op_FPush7,  EOpcodes::OP_FPUSH7,  "fpush7",  1);

//MAKE_SIMPLE_OP(Op_StackSet,  EOpcodes::OP_STACKSET,  "stackset", 2);

MAKE_SIMPLE_OP(Op_FPush, EOpcodes::OP_FPUSH, "fpush",  5);

#endif // FLOAT_H
