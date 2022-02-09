#ifndef ENDIANNESS_H
#define ENDIANNESS_H

#include <QDataStream>
#include <QSysInfo>

class Endianness
{
public:
    static QDataStream::ByteOrder getEndianness();
};

#endif // ENDIANNESS_H
