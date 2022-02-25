#include "opcode.h"

#include <QString>

#include <memory>

class IOpcodeHandlingStrategy
{
public:

    virtual ~IOpcodeHandlingStrategy() = default;

    // Override this for every opcode, to handle it as it needs.
    virtual void Handle() = 0;
};

Opcode::Opcode(QString name, int id, int size)
    : m_name(name)
    , m_id(id)
    , m_size(size)
{}

Opcode::Opcode(Opcode &other)
{
    m_name = other.m_name;
    m_id   = other.m_id;
    m_size = other.m_size;
}

void Opcode::read(QDataStream *stream)
{
    if (m_size != -1)
    {
        unsigned char byte;
        *stream >> byte;

        m_data.push_back(byte);
    }
    else
    {
        const std::string someOpcode = "nop";
        std::unique_ptr<IOpcodeHandlingStrategy> handlingStrategy = CreateOpcodeHandlingStrategy(someOpcode);
        handlingStrategy->Handle();
    }
}
