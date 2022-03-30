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

QString IOpcode::getFormattedBytes()
{
    return QString::number(getOp()) + getData().toHex().toUpper();
}

QString IOpcode::getFormattedData()
{
    if (getOp() == EOpcodes::OP_IPUSH || getOp() == EOpcodes::OP_IPUSH2 || getOp() == EOpcodes::OP_IPUSH3 || getOp() == EOpcodes::OP_SADDI)
    {
        int result;
        char temp[4];

        memset(temp, 0, 4);

        for (int i = 0; i < getData().size(); i++)
        {
            temp[i] = getData()[getData().size() - 1 - i];
        }

        memcpy(&result, &temp, sizeof(int));

        return QString("%1").arg(result);
    }

    if (getOp() == EOpcodes::OP_FPUSH)
    {
        QDataStream str(getData());
        QString result;
        float fl;

        str.setFloatingPointPrecision(QDataStream::SinglePrecision);

        str >> fl;

        if (fl - floor(fl) == 0)
            result = QString("%1.0f").arg(fl);
        else
            result = QString("%1f").arg(fl);

        return result;
    }

    return getData().toHex().toUpper();
}
