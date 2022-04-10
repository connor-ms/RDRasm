#include "util.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "../../lib/Qt-AES/qaesencryption.h"

#include "lzx.h"
#include "zlib.h"

#define CHUNK 16384

QByteArray Util::getAESKey()
{
    QFile key("rdr_key.bin");

    if (!key.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(nullptr, "Error", "Error: Unable to retrieve AES key. Make sure 'key.bin' exists in the root directory.");
        return QByteArray();
    }

    return key.readAll();
}

QByteArray Util::decrypt(QByteArray in)
{
    QByteArray result(in);

    //int oldSize = result.size();

    // pad to nearest 16 bytes for AES to be able to decrypt
    result.resize(result.size() + (16 - (result.size() % 16)));

    for (int i = 0; i < 0x10; i++)
    {
        result = QAESEncryption::Decrypt(QAESEncryption::Aes::AES_256, QAESEncryption::Mode::ECB, result, Util::getAESKey(), QByteArray(), QAESEncryption::ZERO);
    }

    //result.resize(oldSize);

    return result;
}

QByteArray Util::encrypt(QByteArray in)
{
    QByteArray result(in);

    for (int i = 0; i < 0x10; i++)
    {
        result = QAESEncryption::Crypt(QAESEncryption::Aes::AES_256, QAESEncryption::Mode::ECB, result, Util::getAESKey(), QByteArray(), QAESEncryption::ZERO);
    }

    return result;
}

void Util::lzxDecompress(std::vector<uint8_t> &in, std::vector<uint8_t> &out)
{
    struct LZXstate *lzx_state = lzxInit(17);

    unsigned int outputSize = 0;

    int offset = 0;

    while (outputSize != out.size())
    {
        int tmpoutputSize = 0;
        int tmpinputSize = 0;

        if (*(unsigned char *)(&in[0] + offset) == 0xff)
        {
            tmpoutputSize  = *(unsigned char *)(&in[0] + offset + 1) << 8;
            tmpoutputSize |= *(unsigned char *)(&in[0] + offset + 2);
            tmpinputSize   = *(unsigned char *)(&in[0] + offset + 3) << 8;
            tmpinputSize  |= *(unsigned char *)(&in[0] + offset + 4);

            offset += 5;
        }
        else
        {
            tmpoutputSize = 0x8000;
            tmpinputSize = (*(unsigned char *)(&in[0] + offset) << 8) | *(unsigned char *)(&in[0] + offset + 1);
            if (tmpinputSize == 0)
                break;
            offset += 2;
        }

        int res = ::lzxDecompress(lzx_state, &in[0] + offset, &out[0] + outputSize, tmpinputSize, tmpoutputSize);

        if (res != 0)
        {
            QMessageBox::critical(0, "Error", QString("Error: LZX decompression failed! (Error code: %1)").arg(res));
            break;
        }

        offset += tmpinputSize;
        outputSize += tmpoutputSize;
    }

    lzxTeardown(lzx_state);
}

void Util::zlibDecompress(const std::vector<uint8_t>& in, std::vector<uint8_t> &out)
{
    z_stream infstream;

    infstream.zalloc = Z_NULL;
    infstream.zfree  = Z_NULL;
    infstream.opaque = Z_NULL;

    infstream.avail_in  = (uInt)in.size();
    infstream.next_in   = (Bytef *)&in[0];
    infstream.avail_out = (uInt)out.size();
    infstream.next_out  = (Bytef *)&out[0];

    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);
}

void Util::zlibCompress(const std::vector<uint8_t>& in, std::vector<uint8_t> &out)
{
    int32_t ec, flush;
    uint32_t have;
    z_stream strm;

    uint8_t inbuf[CHUNK];
    uint8_t outbuf[CHUNK];

    uint32_t inIndex    = 0;
    uint32_t inIndexOld = 0;

    out.clear();

    // allocate deflate state
    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    ec = deflateInit(&strm, Z_BEST_COMPRESSION);

    if (ec != Z_OK)
    {
        QMessageBox::critical(nullptr, "ZLib Error", QString("ZLib compression error. (%1)").arg(zlibErrorCodeToStr(ec).c_str()));
        return;
    }

    // compress until end of file
    do
    {
        inIndexOld = inIndex;
        inIndex += CHUNK;

        if (inIndex > in.size())
        {
            inIndex = in.size();
            flush = Z_FINISH;
        }
        else
            flush = Z_NO_FLUSH;

        uint32_t size = inIndex - inIndexOld;
        memcpy(inbuf, in.data() + inIndexOld, size);

        strm.avail_in = size;
        strm.next_in  = inbuf;

        do
        {
            strm.avail_out = CHUNK;
            strm.next_out  = outbuf;

            ec = deflate(&strm, flush);

            if (ec == Z_STREAM_ERROR)
            {
                QMessageBox::critical(nullptr, "ZLib Error", QString("ZLib compression error. (%1)").arg(zlibErrorCodeToStr(ec).c_str()));
                return;
            }

            have = CHUNK - strm.avail_out;

            out.insert(out.end(), outbuf, outbuf + have);

        } while (strm.avail_out == 0);

        assert(strm.avail_in == 0);

    } while (flush != Z_FINISH);

    if (ec != Z_STREAM_END)
    {
        QMessageBox::critical(nullptr, "ZLib Error", QString("ZLib compression error. (%1)").arg(zlibErrorCodeToStr(ec).c_str()));
    }

    ec = deflateEnd(&strm);

    if (ec != Z_OK)
    {
        QMessageBox::critical(nullptr, "ZLib Error", QString("ZLib compression error. (%1)").arg(zlibErrorCodeToStr(ec).c_str()));
    }
}

std::string Util::zlibErrorCodeToStr(int32_t errorcode)
{
    switch (errorcode)
    {
        case Z_OK:            return "Z_OK";
        case Z_STREAM_END:    return "Z_STREAM_END";
        case Z_NEED_DICT:     return "Z_NEED_DICT";
        case Z_ERRNO:         return "Z_ERRNO";
        case Z_STREAM_ERROR:  return "Z_STREAM_ERROR";
        case Z_DATA_ERROR:    return "Z_DATA_ERROR";
        case Z_MEM_ERROR:     return "Z_MEM_ERROR";
        case Z_BUF_ERROR:     return "Z_BUF_ERROR";
        case Z_VERSION_ERROR: return "Z_VERSION_ERROR";
    }

    return "UNK_ERR" + QString::number(errorcode).toStdString();
}

unsigned int Util::hash(std::string str, bool lowercase)
{
    uint32_t value = 0, temp = 0;
    unsigned int index = 0;
    bool quoted = false;

    if (str[index] == '"')
    {
        quoted = true;
        index++;
    }

    // convert to lowercase
    if (lowercase)
    {
        std::transform(str.begin(), str.end(), str.begin(),
            [](unsigned char c) { return std::tolower(c); });
    }

    for (; index < str.length(); index++)
    {
        char v = str[index];

        if (quoted && (v == '"')) break;

        if (v == '\\')
            v = '/';

        temp = v;
        temp = temp + value;
        value = temp << 10;
        temp += value;
        value = temp >> 6;
        value = value ^ temp;
    }

    temp = value << 3;
    temp = value + temp;
    uint32_t temp2 = temp >> 11;
    temp = temp2 ^ temp;
    temp2 = temp << 15;

    value = temp2 + temp;

    if (value < 2) value += 2;

    return value;
}

QMap<unsigned int, QString> Util::getNatives()
{
    QMap<unsigned int, QString> nativeMap;

    QFile natives(":/res/rage/natives.txt");

    if (natives.open(QIODevice::ReadOnly))
    {
        QTextStream in(&natives);

        while (!in.atEnd())
        {
            QString line = in.readLine();

            uint32_t hash = Util::hash(line.toStdString());

            nativeMap.insert(hash, line);
        }

        natives.close();
    }
    else
    {
        QMessageBox::critical(0, "Error", "Error: Failed to read natives. Only hashes will be available.");
    }

    QFile customNatives(":/res/rage/custom-natives.txt");

    if (customNatives.open(QIODevice::ReadOnly))
    {
        QTextStream in(&customNatives);

        while (!in.atEnd())
        {
            QString line = in.readLine();

            uint hash = line.split(" : ").at(0).toUInt();
            QString native = line.split(" : ").at(1);

            nativeMap.insert(hash, native);
        }

        customNatives.close();
    }
    else
    {
        QMessageBox::critical(0, "Error", "Error: Failed to read custom natives. Only hashes will be available.");
    }

    return nativeMap;
}

QString Util::getNative(unsigned int key, QMap<unsigned int, QString> natives)
{
    if (natives.count(key) == 1)
    {
        return natives.value(key);
    }
    else
    {
        return QString("UNK_0x%1").arg(key, 0, 16);
    }
}
