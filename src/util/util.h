#ifndef UTIL_H
#define UTIL_H

#include <QByteArray>
#include <QMap>

class Util
{
public:
    static QByteArray getAESKey();
    static void decompressBuffer(unsigned char *in, unsigned char *out, int outsize);

    static unsigned int hash(std::string str, bool lowercase = true);
    static QMap<unsigned int, QString> getNatives();
};

#endif // UTIL_H
