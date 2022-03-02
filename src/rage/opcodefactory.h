#ifndef OPCODEFACTORY_H
#define OPCODEFACTORY_H

#include <memory>
#include <QDataStream>
#include <QDebug>
#include <QVector>

#define OP_REGISTER(opcode) bool opcode::s_registered = OpcodeFactory::Register(opcode::GetFactoryType(), opcode::CreateMethod)
#define REGISTER(className, op, name) public: static std::shared_ptr<IOpcode> CreateMethod() { return std::make_shared<className>(); }\
                                              static EOpcodes GetFactoryType() { return op; }\
                                              virtual QString getName() override { return name; }\
                                              virtual EOpcodes getOp() override { return op; }\
                                      private: static bool s_registered;
#define MAKE_SIMPLE_OP(className, op, str, size) class className : public IOpcode, public RegisteredInFactory<className> { REGISTER(className, op, str) virtual int getSize() override { return size; } }; OP_REGISTER(className)

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
    OP_JR2,      OP_JFR2,       OP_JNER2,   OP_JEQR2,   OP_JLER2,   OP_JLTR2,   OP_JGER2,   OP_JGTR2,
    OP_PGLOBAL3, OP_GLOBALGET3, OP_GLOBALSET3,
    OP_IPUSH3,
    OP_SWITCHR2,
    OP_SPUSH,    OP_SPUSHL, OP_SPUSH0, OP_SCPY,   OP_ITOS,   OP_SADD,   OP_SADDI,  OP_SNCPY,
    OP_CATCH,    OP_THROW,  OP_PCALL,
    OP_RET0R0,   OP_RET0R1, OP_RET0R2, OP_RET0R3, OP_RET1R0, OP_RET1R1, OP_RET1R2, OP_RET1R3,
    OP_RET2R0,   OP_RET2R1, OP_RET2R2, OP_RET2R3, OP_RET3R0, OP_RET3R1, OP_RET3R2, OP_RET3R3,
    OP_PUSHNEG1, OP_PUSH0,  OP_PUSH1,  OP_PUSH2,  OP_PUSH3,  OP_PUSH4,  OP_PUSH5,  OP_PUSH6,  OP_PUSH7,
    OP_FPUSHN1,  OP_FPUSH0, OP_FPUSH1, OP_FPUSH2, OP_FPUSH3, OP_FPUSH4, OP_FPUSH5, OP_FPUSH6, OP_FPUSH7
};

class IOpcode
{
public:
    IOpcode() = default;
    virtual ~IOpcode() = default;

    virtual void     read(QDataStream *stream);
    virtual QString  getName() = 0;
    virtual EOpcodes getOp()   = 0;

    virtual QByteArray   getData()     { return m_data;     }
    virtual unsigned int getLocation() { return m_location; }
    virtual int          getSize()     { return m_size;     }

    virtual int  getPage()         { return m_page; }
    virtual void setPage(int page) { m_page = page; }

    virtual QString getFormattedLocation(); // page:offset
    virtual QString getString();            // full string
    virtual QString getDataString();        // formatted raw data
    virtual QString getArgsString();        // formatted arguments of op

protected:
    QByteArray m_data;
    unsigned int m_location;
    QVector<IOpcode*> m_references;
    int m_size;
    int m_page;
};

class OpcodeFactory
{
public:
    using TCreateMethod = std::shared_ptr<IOpcode>(*)();
    TCreateMethod m_CreateFunc;

public:
    OpcodeFactory() = delete;

    static bool Register(const EOpcodes opcode, TCreateMethod createFunc);
    static std::shared_ptr<IOpcode> Create(const EOpcodes opcode);

private:
    static std::map<EOpcodes, TCreateMethod> s_methods;
};

template <typename T>
class RegisteredInFactory
{
protected:
    static bool s_bRegistered;
};

#endif // OPCODEFACTORY_H
