#include "enter.h"

#include <QDebug>

void Op_Enter::read(QDataStream *stream)
{
    m_delete   = false;
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

QString Op_Enter::getFormattedBytes()
{
    // ignore func name in data string
    QByteArray copy(getData());

    return copy.remove(4, getData().size()).toHex().toUpper();
}

QString Op_Enter::getFormattedData()
{
    // only return func name
    QByteArray copy(getData());

    return copy.remove(0, 4);
}

OP_REGISTER(Op_Enter);
