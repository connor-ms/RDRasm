#ifndef IOPCODE_H
#define IOPCODE_H

#include <QDataStream>

typedef unsigned char byte;

enum EOpcodes
{
    OP_NOP,
    OP_IADD, OP_ISUB, OP_IMUL, OP_IDIV, OP_IMOD, OP_INOT, OP_INEG,   OP_ICMPEQ, OP_ICMPNE, OP_ICMPGT, OP_ICMPGE, OP_ICMPLT, OP_ICMPLE,
    OP_FADD, OP_FSUB, OP_FMUL, OP_FDIV, OP_FMOD, OP_FNEG, OP_FCMPEQ, OP_FCMPNE, OP_FCMPGT, OP_FCMPGE, OP_FCMPLT, OP_FCMPLE,
    OP_VADD, OP_VSUB, OP_VMUL, OP_VDIV, OP_VNEG,
    OP_IBITWISE_AND, OP_IBITWISE_OR, OP_IBITWISE_XOR,
    OP_ITOF, OP_FTOI,
    OP_DUP2,
    OP_PUSH1B, OP_PUSH2B, OP_PUSH3B, OP_IPUSH, OP_FPUSH,
    OP_DUP,
    OP_DROP,
    OP_NATIVE,
    OP_ENTER,
    OP_RET, OP_PGET,  OP_PSET,
    OP_PPEEKSET,
    OP_TOSTACK, OP_FROMSTACK,
    OP_PARRAY,  OP_AGET, OP_ASET,
    OP_PFRAME1,
    OP_GETF, OP_SETF,
    OP_STACKGETP,  OP_STACKGET,   OP_STACKSET,
    OP_IADDIMM1,   OP_PGETIMM1,   OP_PSETIMM1,   OP_IMULIMM1,
    OP_IPUSH2,     OP_IADDIMM2,   OP_PGETIMM2,   OP_PSETIMM2, OP_IMULIMM2,
    OP_ARRAYGETP2, OP_ARRAYGET2,  OP_ARRAYSET2,
    OP_PFRAME2,    OP_FRAMEGET2,  OP_FRAMESET2,
    OP_PSTATIC2,   OP_STATICGET2, OP_STATICSET2, OP_PGLOBAL2,
    OP_GLOBALGET2,
    OP_GLOBALSET2,
    OP_CALL2,    OP_CALL2H1,    OP_CALL2H2, OP_CALL2H3, OP_CALL2H4, OP_CALL2H5, OP_CALL2H6, OP_CALL2H7,
    OP_CALL2H8,  OP_CALL2H9,    OP_CALL2HA, OP_CALL2HB, OP_CALL2HC, OP_CALL2HD, OP_CALL2HE, OP_CALL2HF,
    OP_JMP,      OP_JMPF,       OP_JMPNE,   OP_JMPEQ,   OP_JMPLE,   OP_JMPLT,   OP_JMPGE,   OP_JMPGT,
    OP_PGLOBAL3, OP_GLOBALGET3, OP_GLOBALSET3,
    OP_IPUSH3,
    OP_SWITCHR2,
    OP_SPUSH,    OP_SPUSHL, OP_SPUSH0, OP_SCPY,   OP_ITOS,   OP_SADD,   OP_SADDI,  OP_SNCPY,
    OP_CATCH,    OP_THROW,  OP_PCALL,
    OP_RET0R0,   OP_RET0R1, OP_RET0R2, OP_RET0R3, OP_RET1R0, OP_RET1R1, OP_RET1R2, OP_RET1R3,
    OP_RET2R0,   OP_RET2R1, OP_RET2R2, OP_RET2R3, OP_RET3R0, OP_RET3R1, OP_RET3R2, OP_RET3R3,
    OP_PUSHNEG1, OP_PUSH0,  OP_PUSH1,  OP_PUSH2,  OP_PUSH3,  OP_PUSH4,  OP_PUSH5,  OP_PUSH6,  OP_PUSH7,
    OP_FPUSHN1,  OP_FPUSH0, OP_FPUSH1, OP_FPUSH2, OP_FPUSH3, OP_FPUSH4, OP_FPUSH5, OP_FPUSH6, OP_FPUSH7,
    // helper opcodes, these get ignored when recompiling
    _SPACER, // spacer that goes above new function
    _SUB     // start of subroutine
};

class IOpcode
{
public:
    IOpcode() = default;
    virtual ~IOpcode() = default;

    virtual void     read(QDataStream *stream);
    virtual QString  getName() = 0;
    virtual EOpcodes getOp()   = 0;

    virtual QByteArray   &getData()     { return m_data;     }
    virtual unsigned int getLocation() { return m_location; }
    virtual int          getSize()     { return m_size;     }

    virtual int  getPage()         { return m_page; }
    virtual void setPage(int page) { m_page = page; }

    virtual QString getFormattedLocation(); // page:offset
    virtual QString getFormattedBytes();    // raw data
    virtual QString getFormattedData();     // arguments of op

    virtual QByteArray getFullData();

    // Editing related

    virtual bool getDeleted()             { return m_delete;    }
    virtual void setDeleted(bool deleted) { m_delete = deleted; } // mark for deletion when recompiled

    virtual void setData(QByteArray data) { m_data = data;      }

    virtual void setLocation(int loc)     { m_location = loc;   }

protected:
    QByteArray m_data;
    unsigned int m_location;
    QVector<IOpcode*> m_references;
    int m_size;
    int m_page;
    bool m_delete;
};
#endif // IOPCODE_H
