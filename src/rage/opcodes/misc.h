#ifndef MISC_H
#define MISC_H

#include "../opcodefactory.h"

// mostly undocumented

MAKE_SIMPLE_OP(Op_Nop,  EOpcodes::OP_NOP,  "nop",  1);
MAKE_SIMPLE_OP(Op_Dup,  EOpcodes::OP_DUP,  "dup",  1);
MAKE_SIMPLE_OP(Op_Dup2, EOpcodes::OP_DUP2, "dup2", 1);

MAKE_SIMPLE_OP(Op_Native, EOpcodes::OP_NATIVE, "native", 3);

MAKE_SIMPLE_OP(Op_PPeekSet, EOpcodes::OP_PPEEKSET, "ppeekset", 1);

MAKE_SIMPLE_OP(Op_PArray, EOpcodes::OP_PARRAY, "parray", 2);
MAKE_SIMPLE_OP(Op_AGet,   EOpcodes::OP_AGET,   "aget",   2);
MAKE_SIMPLE_OP(Op_ASet,   EOpcodes::OP_ASET,   "aset",   2);

MAKE_SIMPLE_OP(Op_PFrame1, EOpcodes::OP_PFRAME1, "pframe1", 2);
MAKE_SIMPLE_OP(Op_PFrame2, EOpcodes::OP_PFRAME2, "pframe2", 3);

MAKE_SIMPLE_OP(Op_GetF, EOpcodes::OP_GETF, "getf", 2);
MAKE_SIMPLE_OP(Op_SetF, EOpcodes::OP_SETF, "setf", 2);

MAKE_SIMPLE_OP(Op_PGet,     EOpcodes::OP_PGET,     "pget",     1);
MAKE_SIMPLE_OP(Op_PSet,     EOpcodes::OP_PSET,     "pset",     1);
MAKE_SIMPLE_OP(Op_PGetImm1, EOpcodes::OP_PGETIMM1, "pgetimm1", 2);
MAKE_SIMPLE_OP(Op_PSetImm1, EOpcodes::OP_PSETIMM1, "psetimm1", 2);
MAKE_SIMPLE_OP(Op_PGetImm2, EOpcodes::OP_PGETIMM2, "pgetimm2", 3);
MAKE_SIMPLE_OP(Op_PSetImm2, EOpcodes::OP_PSETIMM2, "psetimm2", 3);

MAKE_SIMPLE_OP(Op_ArrayGet2,  EOpcodes::OP_ARRAYGET2,  "arrayget2",  3);
MAKE_SIMPLE_OP(Op_ArraySet2,  EOpcodes::OP_ARRAYSET2,  "arrayset2",  3);
MAKE_SIMPLE_OP(Op_ArrayGetP2, EOpcodes::OP_ARRAYGETP2, "arraygetp2", 3);

MAKE_SIMPLE_OP(Op_FrameGet2, EOpcodes::OP_FRAMEGET2, "frameget2", 3);
MAKE_SIMPLE_OP(Op_FrameSet2, EOpcodes::OP_FRAMESET2, "frameset2", 3);

MAKE_SIMPLE_OP(Op_PStatic2,   EOpcodes::OP_PSTATIC2,   "pstatic2",   3);
MAKE_SIMPLE_OP(Op_StaticGet2, EOpcodes::OP_STATICGET2, "staticget2", 3);
MAKE_SIMPLE_OP(Op_StaticSet2, EOpcodes::OP_STATICSET2, "staticset2", 3);

MAKE_SIMPLE_OP(Op_PGlobal2,   EOpcodes::OP_PGLOBAL2,   "pglobal2",   3);
MAKE_SIMPLE_OP(Op_GlobalGet2, EOpcodes::OP_GLOBALGET2, "globalget2", 3);
MAKE_SIMPLE_OP(Op_GlobalSet2, EOpcodes::OP_GLOBALSET2, "globalset2", 3);

MAKE_SIMPLE_OP(Op_PGlobal3,   EOpcodes::OP_PGLOBAL3,   "pglobal3",   4);
MAKE_SIMPLE_OP(Op_GlobalGet3, EOpcodes::OP_GLOBALGET3, "globalget3", 4);
MAKE_SIMPLE_OP(Op_GlobalSet3, EOpcodes::OP_GLOBALSET3, "globalset3", 4);

MAKE_SIMPLE_OP(Op_Catch, EOpcodes::OP_CATCH, "catch", 1);
MAKE_SIMPLE_OP(Op_Throw, EOpcodes::OP_THROW, "throw", 1);
MAKE_SIMPLE_OP(Op_PCall, EOpcodes::OP_PCALL, "pcall", 1);

class Op_SwitchR2 : public IOpcode, public RegisteredInFactory<Op_SwitchR2>
{
    REGISTER(Op_SwitchR2, EOpcodes::OP_SWITCHR2, "switchr2")
public:
    virtual void read(QDataStream *stream) override;
};

#endif // MISC_H
