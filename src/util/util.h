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
    static QMap<unsigned int, QString> getNatives(); // generates map of known native names
    static QString getNative(unsigned int key, QMap<unsigned int, QString> natives); // returns native name
};

#endif // UTIL_H
