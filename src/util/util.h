#ifndef UTIL_H
#define UTIL_H

#include <QByteArray>

class Util
{
public:
    static QByteArray getAESKey();
    static void decompressBuffer(unsigned char *in, unsigned char *out, int outsize);
};

#endif // UTIL_H
