#include "opcodefactory.h"

#include <memory>
#include <map>
#include <iostream>

bool OpcodeFactory::Register(const EOpcodes op, TCreateMethod createFunc)
{
    if (auto it = s_methods.find(op); it == s_methods.end())
    {
        s_methods[op] = createFunc;
        return true;
    }
    return false;
}

std::shared_ptr<IOpcode> OpcodeFactory::Create(const EOpcodes name)
{
    if (auto it = s_methods.find(name); it != s_methods.end())
        return it->second();

    return nullptr;
}

template <typename T>
bool RegisteredInFactory<T>::s_bRegistered = OpcodeFactory::Register(T::GetFactoryName(), T::CreateMethod);

std::map<EOpcodes, OpcodeFactory::TCreateMethod> OpcodeFactory::s_methods;
