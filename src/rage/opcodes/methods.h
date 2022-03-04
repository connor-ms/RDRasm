#ifndef METHODS_H
#define METHODS_H

#include "../opcodefactory.h"

MAKE_SIMPLE_OP(Op_Call2,   EOpcodes::OP_CALL2,   "call2",   3); // call imm2
MAKE_SIMPLE_OP(Op_Call2H1, EOpcodes::OP_CALL2H1, "call2h1", 3); // call (imm2|0x1000)
MAKE_SIMPLE_OP(Op_Call2H2, EOpcodes::OP_CALL2H2, "call2h2", 3); // call (imm2|0x2000)
MAKE_SIMPLE_OP(Op_Call2H3, EOpcodes::OP_CALL2H3, "call2h3", 3); // call (imm2|0x3000)
MAKE_SIMPLE_OP(Op_Call2H4, EOpcodes::OP_CALL2H4, "call2h4", 3); // call (imm2|0x4000)
MAKE_SIMPLE_OP(Op_Call2H5, EOpcodes::OP_CALL2H5, "call2h5", 3); // call (imm2|0x5000)
MAKE_SIMPLE_OP(Op_Call2H6, EOpcodes::OP_CALL2H6, "call2h6", 3); // call (imm2|0x6000)
MAKE_SIMPLE_OP(Op_Call2H7, EOpcodes::OP_CALL2H7, "call2h7", 3); // call (imm2|0x7000)
MAKE_SIMPLE_OP(Op_Call2H8, EOpcodes::OP_CALL2H8, "call2h8", 3); // call (imm2|0x8000)
MAKE_SIMPLE_OP(Op_Call2H9, EOpcodes::OP_CALL2H9, "call2h9", 3); // call (imm2|0x9000)
MAKE_SIMPLE_OP(Op_Call2HA, EOpcodes::OP_CALL2HA, "call2ha", 3); // call (imm2|0xA000)
MAKE_SIMPLE_OP(Op_Call2HB, EOpcodes::OP_CALL2HB, "call2hb", 3); // call (imm2|0xB000)
MAKE_SIMPLE_OP(Op_Call2HC, EOpcodes::OP_CALL2HC, "call2hc", 3); // call (imm2|0xC000)
MAKE_SIMPLE_OP(Op_Call2HD, EOpcodes::OP_CALL2HD, "call2hd", 3); // call (imm2|0xD000)
MAKE_SIMPLE_OP(Op_Call2HE, EOpcodes::OP_CALL2HE, "call2he", 3); // call (imm2|0xE000)
MAKE_SIMPLE_OP(Op_Call2HF, EOpcodes::OP_CALL2HF, "call2hf", 3); // call (imm2|0xF000)

MAKE_SIMPLE_OP(Op_Ret0R0, EOpcodes::OP_RET0R0, "ret0r0", 1); // return: 0 parameters, 0 results
MAKE_SIMPLE_OP(Op_Ret0R1, EOpcodes::OP_RET0R1, "ret0r1", 1); // return: 0 parameters, 1 results
MAKE_SIMPLE_OP(Op_Ret0R2, EOpcodes::OP_RET0R2, "ret0r2", 1); // return: 0 parameters, 2 results
MAKE_SIMPLE_OP(Op_Ret0R3, EOpcodes::OP_RET0R3, "ret0r3", 1); // return: 0 parameters, 3 results
MAKE_SIMPLE_OP(Op_Ret1R0, EOpcodes::OP_RET1R0, "ret1r0", 1); // return: 1 parameters, 0 results
MAKE_SIMPLE_OP(Op_Ret1R1, EOpcodes::OP_RET1R1, "ret1r1", 1); // return: 1 parameters, 1 results
MAKE_SIMPLE_OP(Op_Ret1R2, EOpcodes::OP_RET1R2, "ret1r2", 1); // return: 1 parameters, 2 results
MAKE_SIMPLE_OP(Op_Ret1R3, EOpcodes::OP_RET1R3, "ret1r3", 1); // return: 1 parameters, 3 results
MAKE_SIMPLE_OP(Op_Ret2R0, EOpcodes::OP_RET2R0, "ret2r0", 1); // return: 2 parameters, 0 results
MAKE_SIMPLE_OP(Op_Ret2R1, EOpcodes::OP_RET2R1, "ret2r1", 1); // return: 2 parameters, 1 results
MAKE_SIMPLE_OP(Op_Ret2R2, EOpcodes::OP_RET2R2, "ret2r2", 1); // return: 2 parameters, 2 results
MAKE_SIMPLE_OP(Op_Ret2R3, EOpcodes::OP_RET2R3, "ret2r3", 1); // return: 2 parameters, 3 results
MAKE_SIMPLE_OP(Op_Ret3R0, EOpcodes::OP_RET3R0, "ret3r0", 1); // return: 3 parameters, 0 results
MAKE_SIMPLE_OP(Op_Ret3R1, EOpcodes::OP_RET3R1, "ret3r1", 1); // return: 3 parameters, 1 results
MAKE_SIMPLE_OP(Op_Ret3R2, EOpcodes::OP_RET3R2, "ret3r2", 1); // return: 3 parameters, 2 results
MAKE_SIMPLE_OP(Op_Ret3R3, EOpcodes::OP_RET3R3, "ret3r3", 1); // return: 3 parameters, 3 results

MAKE_SIMPLE_OP(Op_Jmp,   EOpcodes::OP_JMP,   "jmp",   3); // jump relative
MAKE_SIMPLE_OP(Op_JmpF,  EOpcodes::OP_JMPF,  "jmpf",  3); // jump false
MAKE_SIMPLE_OP(Op_JmpNe, EOpcodes::OP_JMPNE, "jmpne", 3); // jump not equal
MAKE_SIMPLE_OP(Op_JmpEq, EOpcodes::OP_JMPEQ, "jmpeq", 3); // jump equal
MAKE_SIMPLE_OP(Op_JmpLe, EOpcodes::OP_JMPLE, "jmple", 3); // jump <=
MAKE_SIMPLE_OP(Op_JmpLt, EOpcodes::OP_JMPLT, "jmplt", 3); // jump <
MAKE_SIMPLE_OP(Op_JmpGe, EOpcodes::OP_JMPGE, "jmpge", 3); // jump >=
MAKE_SIMPLE_OP(Op_JmpGt, EOpcodes::OP_JMPGT, "jmpgt", 3); // jump >

MAKE_SIMPLE_OP(Op_Ret, EOpcodes::OP_RET, "ret", 3); // end of func

class Op_Enter : public IOpcode, public RegisteredInFactory<Op_Enter>
{
    REGISTER(Op_Enter, EOpcodes::OP_ENTER, "enter")
public:
    virtual void read(QDataStream *stream) override;
    virtual QString getDataString() override;
    virtual QString getArgsString() override;
    virtual QString getString() override;
};

#endif // METHODS_H
