#ifndef UTIL_H
#define UTIL_H

#include <QByteArray>
#include <QMap>

class Util
{
public:
    static QByteArray getAESKey();

    static QByteArray decrypt(QByteArray in);
    static QByteArray encrypt(QByteArray in);

    static QByteArray lzxDecompress(QByteArray in, int outSize);
    static QByteArray lzxCompress(QByteArray in);

    static QByteArray zlibDecompress(QByteArray in, int outSize);
    static QByteArray zlibCompress(QByteArray in);
    static std::string zlibErrorCodeToStr(int32_t errorcode);

    static unsigned int hash(std::string str, bool lowercase = true);
    static QMap<unsigned int, QString> getNatives(); // generates map of known native names
    static QString getNative(unsigned int key, QMap<unsigned int, QString> natives); // returns native name
};

#endif // UTIL_H
