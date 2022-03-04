#ifndef OPCODEFACTORY_H
#define OPCODEFACTORY_H

#include <memory>

#include "iopcode.h"

#define OP_REGISTER(opcode) bool opcode::s_registered = OpcodeFactory::Register(opcode::GetFactoryType(), opcode::CreateMethod)
#define REGISTER(className, op, name) public: static std::shared_ptr<IOpcode> CreateMethod() { return std::make_shared<className>(); }\
                                              static EOpcodes GetFactoryType() { return op; }\
                                              virtual QString getName() override { return name; }\
                                              virtual EOpcodes getOp() override { return op; }\
                                      private: static bool s_registered;
#define MAKE_SIMPLE_OP(className, op, str, size) class className : public IOpcode, public RegisteredInFactory<className> { REGISTER(className, op, str) virtual int getSize() override { return size; } }; OP_REGISTER(className)

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
