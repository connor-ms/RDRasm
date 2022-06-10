#include "misc.h"

#include <QDebug>

void Op_SwitchR2::read(QDataStream *stream)
{
    m_delete   = false;
    m_location = stream->device()->pos() - 1;

    byte b1, b2;

    int size = 0;

    *stream >> b1;
    size++;

    m_data.push_back(b1);

    // TODO: correctly implement switches

    for (int i = 0; i < b1 * 6; i++)
    {
        *stream >> b2;
        m_data.push_back(b2);
        size += 1;
    }

    m_size = size;
}

OP_REGISTER(Op_SwitchR2);
