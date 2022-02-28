#include "methods.h"

void Op_Enter::read(QDataStream *stream)
{
    m_location = stream->device()->pos();

    QString str;
    byte b;

    // Read first 4 bytes
    for (int i = 0; i < 4; i++)
    {
        *stream >> b;
        m_data.push_back(b);
    }

    // Last byte is size of method name, if exists
    for (int i = 0; i < b; i++)
    {
        byte stringByte;
        *stream >> stringByte;

        //str.append(stringByte);
        m_data.push_back(stringByte);
    }

    m_size = 4 + b;

    //qDebug() << "function: " << m_data;
}

OP_REGISTER(Op_Enter);
