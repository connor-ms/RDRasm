#include "string.h"

void Op_SPush::read(QDataStream *stream)
{
    m_location = stream->device()->pos() - 1;

    byte b;

    *stream >> b;

    m_data.push_back(b);

    for (int i = 0; i < b; i++)
    {
        byte stringByte;
        *stream >> stringByte;

        if (stringByte == 0x0A)
        {
            m_data.push_back("\\n");
        }
        else
        {
            m_data.push_back(stringByte);
        }
    }

    m_size = 2 + b;
}

QString Op_SPush::getDataString()
{
    // ignore string in data array
    return QString::number(getOp()) + QString::number(getData()[0], 16).toUpper();
}

QString Op_SPush::getArgsString()
{
    // only return string
    return "\"" + QString(getData().remove(0, 1)) + "\"";
}

void Op_SPushL::read(QDataStream *stream)
{
    // todo
}

OP_REGISTER(Op_SPush);
OP_REGISTER(Op_SPushL);
