#include "misc.h"

void Op_SwitchR2::read(QDataStream *stream)
{
    m_location = stream->device()->pos() - 1;

    byte b1;
    int temp;
    short temp2;

    int size = 0;

    *stream >> b1;
    size++;

    m_data.push_back(b1);

    for (int i = 0; i < b1; i++)
    {
        *stream >> temp;
        *stream >> temp2;

        m_data.push_back(temp);
        m_data.push_back(temp2);

        size += sizeof(int);
        size += sizeof(short);
    }

    m_size = size;
}

OP_REGISTER(Op_SwitchR2);
