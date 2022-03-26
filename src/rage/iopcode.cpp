#include "iopcode.h"

void IOpcode::read(QDataStream *stream)
{
    m_location = stream->device()->pos() - 1;

    byte b;

    for (int i = 0; i < getSize() - 1; i++)
    {
        *stream >> b;
        m_data.push_back(b);
    }
}

QString IOpcode::getFormattedLocation()
{
    QString page = QString::number(getPage(), 16).rightJustified(5, '0').toUpper();
    QString loc = QString::number(getLocation(), 16).rightJustified(7, '0').toUpper();
    return page + ":" + loc;
}

QString IOpcode::getDataString()
{
    return QString::number(getOp()) + getData().toHex().toUpper();
}

QString IOpcode::getArgsString()
{
    return getData().toHex().toUpper();
}
