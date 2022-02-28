#include "string.h"

void Op_SPush::read(QDataStream *stream)
{
    m_location = stream->device()->pos();

    QString str;
    byte b;

    *stream >> b;

    m_data.push_back(b);

    for (int i = 0; i < b; i++)
    {
        byte stringByte;
        *stream >> stringByte;

        //str.append(stringByte);
        m_data.push_back(stringByte);
    }

    m_size = 2 + b;

    //qDebug() << "string: " << m_data;
}

void Op_SPushL::read(QDataStream *stream)
{

}


OP_REGISTER(Op_SPush);
OP_REGISTER(Op_SPushL);
