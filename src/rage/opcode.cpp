#include "opcode.h"

Opcode::Opcode(OpcodeBase base) :
    OpcodeBase(base.m_name, base.m_initialSize)
{
}

QString Opcode::string()
{
    return QString("");
}
