#include "methods.h"

void Op_Enter::read(QDataStream *stream)
{
    m_location = stream->device()->pos() - 1;

    byte b;

    // Read first 4 bytes
    for (int i = 0; i < 4; i++)
    {
        *stream >> b;
        m_data.push_back(b);
    }

    // Read function name, if it exists
    for (int i = 0; i < b; i++)
    {
        byte stringByte;
        *stream >> stringByte;

        m_data.push_back(stringByte);
    }

    m_size = 4 + b;
}

QString Op_Enter::getDataString()
{
    // ignore func name in data string
    return getData().remove(4, getData().size()).toHex();
}

QString Op_Enter::getArgsString()
{
    // only return func name
    return getData().remove(0, 4);
}

OP_REGISTER(Op_Enter);
