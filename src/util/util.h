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

    static void lzxDecompress(std::vector<uint8_t> &in, std::vector<uint8_t> &out);
    static void lzxCompress(std::vector<uint8_t> &in, std::vector<uint8_t> &out);

    static void zlibDecompress(const std::vector<uint8_t>& in, std::vector<uint8_t> &out);
    static void zlibCompress(const std::vector<uint8_t>& in, std::vector<uint8_t> &out);
    static std::string zlibErrorCodeToStr(int32_t errorcode);

    static unsigned int hash(std::string str, bool lowercase = true);
    static QMap<unsigned int, QString> getNatives(); // generates map of known native names
    static QString getNative(unsigned int key, QMap<unsigned int, QString> natives); // returns native name
};

#endif // UTIL_H
