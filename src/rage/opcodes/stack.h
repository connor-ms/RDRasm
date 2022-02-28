#ifndef STACK_H
#define STACK_H

#include "../opcodefactory.h"

MAKE_SIMPLE_OP(Op_Drop,      EOpcodes::OP_DROP, "drop", 1);
MAKE_SIMPLE_OP(Op_ToStack,   EOpcodes::OP_TOSTACK, "tostack", 1);
MAKE_SIMPLE_OP(Op_FromStack, EOpcodes::OP_FROMSTACK, "fromstack", 1);

MAKE_SIMPLE_OP(Op_StackGetP, EOpcodes::OP_STACKGETP, "stackgetp", 2);
MAKE_SIMPLE_OP(Op_StackGet,  EOpcodes::OP_STACKGET,  "stackget",  2);
MAKE_SIMPLE_OP(Op_StackSet,  EOpcodes::OP_STACKSET,  "stackset",  2);

#endif // STACK_H
