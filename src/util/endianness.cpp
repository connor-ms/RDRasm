#include "endianness.h"

QDataStream::ByteOrder Endianness::getEndianness()
{
    if (QSysInfo::ByteOrder == QSysInfo::BigEndian)
    {
        return QDataStream::BigEndian;
    }
    else
    {
        return QDataStream::LittleEndian;
    }
}
