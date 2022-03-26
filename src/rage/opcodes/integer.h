#ifndef INTEGER_H
#define INTEGER_H

#include "../opcodefactory.h"

// Integer stack operations, takes most recent stack additions
MAKE_SIMPLE_OP(Op_IAdd,   EOpcodes::OP_IADD,   "iadd",   1); // +
MAKE_SIMPLE_OP(Op_ISub,   EOpcodes::OP_ISUB,   "isub",   1); // -
MAKE_SIMPLE_OP(Op_IMul,   EOpcodes::OP_IMUL,   "imul",   1); // *
MAKE_SIMPLE_OP(Op_IDiv,   EOpcodes::OP_IDIV,   "idiv",   1); // /
MAKE_SIMPLE_OP(Op_IMod,   EOpcodes::OP_IMOD,   "imod",   1); // %
MAKE_SIMPLE_OP(Op_INot,   EOpcodes::OP_INOT,   "inot",   1); // logical not
MAKE_SIMPLE_OP(Op_INeg,   EOpcodes::OP_INEG,   "ineg",   1); // ~
MAKE_SIMPLE_OP(Op_ICmpEq, EOpcodes::OP_ICMPEQ, "icmpeq", 1); // ==
MAKE_SIMPLE_OP(Op_ICmpNe, EOpcodes::OP_ICMPNE, "icmpne", 1); // !=
MAKE_SIMPLE_OP(Op_ICmpGt, EOpcodes::OP_ICMPGT, "icmpgt", 1); // >
MAKE_SIMPLE_OP(Op_ICmpGe, EOpcodes::OP_ICMPGE, "icmpge", 1); // >=
MAKE_SIMPLE_OP(Op_ICmpLt, EOpcodes::OP_ICMPLT, "icmplt", 1); // <
MAKE_SIMPLE_OP(Op_ICmpLe, EOpcodes::OP_ICMPLE, "icmple", 1); // <=

MAKE_SIMPLE_OP(Op_IBitwiseAnd, EOpcodes::OP_IBITWISE_AND, "ibitwise_and", 1); // &
MAKE_SIMPLE_OP(Op_IBitwiseOr,  EOpcodes::OP_IBITWISE_OR,  "ibitwise_or",  1); // |
MAKE_SIMPLE_OP(Op_IBitwiseXor, EOpcodes::OP_IBITWISE_XOR, "ibitwise_xor", 1); // ^
MAKE_SIMPLE_OP(Op_IToF,        EOpcodes::OP_ITOF,         "itof",         1); // int to float
MAKE_SIMPLE_OP(Op_IToS,        EOpcodes::OP_ITOS,         "itos",         2); // int to string

// Push integer val to stack
MAKE_SIMPLE_OP(Op_PushNeg1, EOpcodes::OP_PUSHNEG1, "pushneg1", 1);
MAKE_SIMPLE_OP(Op_Push0,    EOpcodes::OP_PUSH0,    "push0",    1);
MAKE_SIMPLE_OP(Op_Push1,    EOpcodes::OP_PUSH1,    "push1",    1);
MAKE_SIMPLE_OP(Op_Push2,    EOpcodes::OP_PUSH2,    "push2",    1);
MAKE_SIMPLE_OP(Op_Push3,    EOpcodes::OP_PUSH3,    "push3",    1);
MAKE_SIMPLE_OP(Op_Push4,    EOpcodes::OP_PUSH4,    "push4",    1);
MAKE_SIMPLE_OP(Op_Push5,    EOpcodes::OP_PUSH5,    "push5",    1);
MAKE_SIMPLE_OP(Op_Push6,    EOpcodes::OP_PUSH6,    "push6",    1);
MAKE_SIMPLE_OP(Op_Push7,    EOpcodes::OP_PUSH7,    "push7",    1);

MAKE_SIMPLE_OP(Op_IPush2,   EOpcodes::OP_IPUSH2,   "ipush2",   3); // push next 2 bytes
MAKE_SIMPLE_OP(Op_IPush3,   EOpcodes::OP_IPUSH3,   "ipush3",   4); // push next 3 bytes
MAKE_SIMPLE_OP(Op_IPush,    EOpcodes::OP_IPUSH,    "ipush",    5); // push next 4 bytes

// Push x bytes to stack
MAKE_SIMPLE_OP(Op_Push1B, EOpcodes::OP_PUSH1B, "push1b", 2);
MAKE_SIMPLE_OP(Op_Push2B, EOpcodes::OP_PUSH2B, "push2b", 3);
MAKE_SIMPLE_OP(Op_Push3B, EOpcodes::OP_PUSH3B, "push3b", 4);

// Undocumented
MAKE_SIMPLE_OP(Op_IAddImm1, EOpcodes::OP_IADDIMM1, "iaddimm1", 2);
MAKE_SIMPLE_OP(Op_IAddImm2, EOpcodes::OP_IADDIMM2, "iaddimm2", 3);
MAKE_SIMPLE_OP(Op_IMulImm1, EOpcodes::OP_IMULIMM1, "imulimm1", 2);
MAKE_SIMPLE_OP(Op_IMulImm2, EOpcodes::OP_IMULIMM2, "imulimm1", 3);

#endif // INTEGER_H
