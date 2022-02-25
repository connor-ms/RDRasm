#include "opcodebuilder.h"

#define ADD(n,s) m_opcodes.push_back(Opcode(n, m_opcodes.size(), s));

OpcodeBuilder::OpcodeBuilder()
{
    ADD("nop", 1);
    ADD("iadd", 1);    // 1 - integer +, stack operands
    ADD("isub", 1);    // 2 - integer -, stack operandes
    ADD("imul", 1);    // 3 - integer *, stack operands
    ADD("idiv", 1);    // 4 - integer /, stack operands
    ADD("imod", 1);    // 5 - integer %, stack operands
    ADD("inot", 1);    // 6 - logical not, stack operand
    ADD("ineg", 1);    // 7 - integer negate, stack operand
    ADD("icmpeq", 1);      // 8 - integer compare =, stack operands
    ADD("icmpne", 1);      // 9 - integer compare !=, stack operands
    ADD("icmpgt", 1);      // 10 - integer compare >, stack operands
    ADD("icmpge", 1);      // 11 - integer compare >=, stack operands
    ADD("icmplt", 1);      // 12 - integer compare <, stack operands
    ADD("icmple", 1);      // 13 - integer compare <=, stack operands
    ADD("fadd", 1);    // 14 - float +, stack operands
    ADD("fsub", 1);    // 15
    ADD("fmul", 1);    // 16
    ADD("fdiv", 1);    // 17
    ADD("fmod", 1);    // 18
    ADD("fneg", 1);    // 19
    ADD("fcmpeq", 1);      // 20
    ADD("fcmpne", 1);      // 21
    ADD("fcmpgt", 1);      // 22
    ADD("fcmpge", 1);      // 23
    ADD("fcmplt", 1);      // 24
    ADD("fcmple", 1);      // 25
    ADD("vadd", 1);    // 26 - vector +, stack operands
    ADD("vsub", 1);    // 27
    ADD("vmul", 1);    // 28
    ADD("vdiv", 1);    // 29
    ADD("vneg", 1);    // 30
    ADD("ibitwise_and", 1);    // 31 - integer &, stack operands
    ADD("ibitwise_or", 1);     // 32
    ADD("ibitwise_xor", 1);    // 33
    ADD("itof", 1);    // 34
    ADD("ftoi", 1);    // 35
    ADD("dup2", 1);    // 36
    ADD("Push1b", 2);      // 37   ipush imm1
    ADD("Push2b", 3);      // 38   ipush imm1, imm1
    ADD("Push3b", 4);      // 39   ipush imm1, imm1, imm1
    ADD("PushInt", 5);     // 40
    ADD("PushFloat", 5);   // 41
    ADD("dup", 1);     // 42
    ADD("drop", 1);    // 43
    ADD("native", 3);      // 44
    ADD("enter", 5);       // 45   - 5 + last byte
    ADD("ret", 3);     // 46
    ADD("pget", 1);    // 47
    ADD("pset", 1);    // 48
    ADD("ppeekset", 1);    // 49
    ADD("tostack", 1);     // 50
    ADD("fromstack", 1);   // 51
    ADD("parray", 2);      // 52
    ADD("aget", 2);    // 53
    ADD("aset", 2);    // 54
    ADD("pframe1", 2);     // 55
    ADD("getf", 2);    // 56 - get value from the stack frame, immediate 1-byte offset
    ADD("setf", 2);    // 57 - set value to the stack frame, immediate 1-byte offset
    ADD("stackgetp", 2);   // 58
    ADD("stackget", 2);    // 59
    ADD("stackset", 2);    // 60
    ADD("iaddimm1", 2);    // 61
    ADD("pgetimm1", 2);    // 62
    ADD("psetimm1", 2);    // 63
    ADD("imulimm1", 2);    // 64
    ADD("ipush2", 3);      // 65
    ADD("iaddimm2", 3);    // 66
    ADD("pgetimm2", 3);    // 67
    ADD("psetimm2", 3);    // 68
    ADD("imulimm2", 3);    // 69
    ADD("arraygetp2", 3);  // 70
    ADD("arrayget2", 3);   // 71
    ADD("arrayset2", 3);   // 72
    ADD("pframe2", 3);     // 73
    ADD("frameget2", 3);   // 74
    ADD("frameset2", 3);   // 75
    ADD("pstatic2", 3);    // 76
    ADD("staticget2", 3);  // 77
    ADD("staticset2", 3);  // 78
    ADD("pglobal2", 3);    // 79
    ADD("globalget2", 3);  // 80
    ADD("globalset2", 3);  // 81
    ADD("call2", 3);       // 82   call imm2
    ADD("call2h1", 3);     // 83   call (imm2|0x1000)
    ADD("call2h2", 3);     // 84   call (imm2|0x2000)
    ADD("call2h3", 3);     // 85   call (imm2|0x3000)
    ADD("call2h4", 3);     // 86   call (imm2|0x4000)
    ADD("call2h5", 3);     // 87   call (imm2|0x5000)
    ADD("call2h6", 3);     // 88   call (imm2|0x6000)
    ADD("call2h7", 3);     // 89   call (imm2|0x7000)
    ADD("call2h8", 3);     // 90   call (imm2|0x8000)
    ADD("call2h9", 3);     // 91   call (imm2|0x9000)
    ADD("call2ha", 3);     // 92   call (imm2|0xA000)
    ADD("call2hb", 3);     // 93   call (imm2|0xB000)
    ADD("call2hc", 3);     // 94   call (imm2|0xC000)
    ADD("call2hd", 3);     // 95   call (imm2|0xD000)
    ADD("call2he", 3);     // 96   call (imm2|0xE000)
    ADD("call2hf", 3);     // 97   call (imm2|0xF000)
    ADD("jr2", 3);     // 98   jump relative signed imm2
    ADD("jfr2", 3);    // 99   jump if false relative signed imm2
    ADD("jner2", 3);       // 100
    ADD("jeqr2", 3);       // 101
    ADD("jler2", 3);       // 102  TODO: <= or > ?
    ADD("jltr2", 3);       // 103  TODO: < or >= ?
    ADD("jger2", 3);       // 104  TODO: >= or < ?
    ADD("jgtr2", 3);       // 105  TODO: > or <= ?
    ADD("pglobal3", 4);    // 106
    ADD("globalget3", 4);  // 107
    ADD("globalset3", 4);  // 108
    ADD("ipush3", 4);      // 109
    ADD("switchr2", 0);    // 110  length = 2 + byte[1]*6
    ADD("spush", 0);       // 111  length = 2 + byte[1]
    ADD("spushl", 0);      // 112  length = 5 + dword[1]
    ADD("spush0", 1);      // 113  push ""
    ADD("scpy", 2);    // 114
    ADD("itos", 2);    // 115
    ADD("sadd", 2);    // 116
    ADD("saddi", 2);       // 117
    ADD("sncpy", 1);       // 118
    ADD("catch", 1);       // 119
    ADD("throw", 1);       // 120
    ADD("pcall", 1);       // 121
    ADD("ret0r0", 1);      // 122 - return: 0 parameters, 0 results
    ADD("ret0r1", 1);      // 123 - return: 0 parameters, 1 result
    ADD("ret0r2", 1);      // 124 - return: 0 parameters, 2 results
    ADD("ret0r3", 1);      // 125 - return: 0 parameters, 3 results
    ADD("ret1r0", 1);      // 126 - return: 1 parameter, 0 results
    ADD("ret1r1", 1);      // 127 - return: 1 parameter, 1 result
    ADD("ret1r2", 1);      // 128 - return: 1 parameter, 2 results
    ADD("ret1r3", 1);      // 129 - return: 1 parameter, 3 results
    ADD("ret2r0", 1);      // 130 - return: 2 parameters, 0 results
    ADD("ret2r1", 1);      // 131 - return: 2 parameters, 1 result
    ADD("ret2r2", 1);      // 132 - return: 2 parameters, 2 results
    ADD("ret2r3", 1);      // 133 - return: 2 parameters, 3 results
    ADD("ret3r0", 1);      // 134 - return: 3 parameters, 0 results
    ADD("ret3r1", 1);      // 135 - return: 3 parameters, 1 result
    ADD("ret3r2", 1);      // 136 - return: 3 parameters, 2 results
    ADD("ret3r3", 1);      // 137 - return: 3 parameters, 3 results
    ADD("Push -1", 1);      // 138  push -1
    ADD("Push 0", 1);       // 139  push 0
    ADD("Push 1", 1);       // 140  push 1
    ADD("Push 2", 1);       // 141  push 2
    ADD("Push 3", 1);       // 142  push 3
    ADD("Push 4", 1);       // 143  push 4
    ADD("Push 5", 1);       // 144  push 5
    ADD("Push 6", 1);       // 145  push 6
    ADD("Push 7", 1);       // 146  push 7
    ADD("fimmn1", 1);      // 147  push -1.0f
    ADD("fimm0", 1);       // 148  push 0.0f
    ADD("fimm1", 1);       // 149  push 1.0f
    ADD("fimm2", 1);       // 150  push 2.0f
    ADD("fimm3", 1);       // 151  push 3.0f
    ADD("fimm4", 1);       // 152  push 4.0f
    ADD("fimm5", 1);       // 153  push 5.0f
    ADD("fimm6", 1);       // 154  push 6.0f
    ADD("fimm7", 1)    // 155  push 7.0f
}

Opcode OpcodeBuilder::createOpcode(int index, QDataStream *stream)
{
    return Opcode(m_opcodes.at(index), stream);
}
