#include "util.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "crypto/aes256.h"
#include "crypto/lzx.h"
#include "crypto/xcompress.h"
#include "crypto/zlib.h"

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

    if (result.size() == 0)
        return QByteArray();

    uint32_t inputCount = result.size() & -16;
    if (inputCount > 0)
    {
        aes256_context ctx;
        aes256_init(&ctx, const_cast<uint8_t*>((uint8_t*)Util::getAESKey().data()));

        for (uint32_t i = 0; i < inputCount; i += 16)
        {
            for (uint32_t b = 0; b < 16; b++)
                aes256_decrypt_ecb(&ctx, (uint8_t*)result.data() + i);
        }

        aes256_done(&ctx);
        return result;
    }

    return QByteArray();
}

QByteArray Util::encrypt(QByteArray in)
{
    QByteArray result(in);

    if (in.size() == 0)
        return QByteArray();

    uint32_t inputCount = in.size() & -16;
    if (inputCount > 0)
    {
        aes256_context ctx;
        aes256_init(&ctx, const_cast<uint8_t*>((uint8_t*)Util::getAESKey().data()));

        for (uint32_t i = 0; i < inputCount; i += 16)
        {
            for (uint32_t b = 0; b < 16; b++)
                aes256_encrypt_ecb(&ctx, (uint8_t*)result.data() + i);
        }

        aes256_done(&ctx);
        return result;
    }

    return QByteArray();
}


QByteArray Util::lzxDecompress(QByteArray in, int outSize)
{
    QByteArray result;
    result.resize(outSize);

    struct LZXstate *lzx_state = lzxInit(17);

    int outputSize = 0;

    int offset = 0;

    while (outputSize != result.size())
    {
        int tmpoutputSize = 0;
        int tmpinputSize = 0;

        if (*(unsigned char *)(in.data() + offset) == 0xff)
        {
            tmpoutputSize  = *(unsigned char *)(in.data() + offset + 1) << 8;
            tmpoutputSize |= *(unsigned char *)(in.data() + offset + 2);
            tmpinputSize   = *(unsigned char *)(in.data() + offset + 3) << 8;
            tmpinputSize  |= *(unsigned char *)(in.data() + offset + 4);

            offset += 5;
        }
        else
        {
            tmpoutputSize = 0x8000;
            tmpinputSize = (*(unsigned char *)(in.data() + offset) << 8) | *(unsigned char *)(in.data() + offset + 1);
            if (tmpinputSize == 0)
                break;
            offset += 2;
        }

        int res = ::lzxDecompress(lzx_state, (unsigned char*)in.data() + offset, (unsigned char*)result.data() + outputSize, tmpinputSize, tmpoutputSize);

        if (res != 0)
        {
            QMessageBox::critical(0, "Error", QString("Error: LZX decompression failed! (Error code: %1)").arg(res));
            break;
        }

        offset += tmpinputSize;
        outputSize += tmpoutputSize;
    }

    lzxTeardown(lzx_state);

    return result;
}

QByteArray Util::lzxCompress(QByteArray in)
{
    QByteArray compressedData;
    xCompress compression;
    int compressedLen = 0;

    compressedData.resize(in.size() + 8);

    compression.xCompressInit();
    compression.Compress(reinterpret_cast<unsigned char*>(in.data()), in.size(), (uint8_t*)compressedData.data() + 8, (int32_t*)&compressedLen);

    compressedData.remove(0, 4);

    QDataStream str(&compressedData, QIODevice::WriteOnly);
    str << compressedLen;

    compressedData.prepend("\x0F\xF5\x12\xF1"); // lzx header?

    compressedLen += 8;

    compressedData.resize(compressedLen);

    return compressedData;
}

QByteArray Util::zlibDecompress(QByteArray in, int outSize)
{
    QByteArray result;
    result.resize(outSize);

    z_stream infstream;

    infstream.zalloc = Z_NULL;
    infstream.zfree  = Z_NULL;
    infstream.opaque = Z_NULL;

    infstream.avail_in  = (uInt)in.size();
    infstream.next_in   = (Bytef *)in.data();
    infstream.avail_out = (uInt)outSize;
    infstream.next_out  = (Bytef *)result.data();

    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);

    return result;
}

QByteArray Util::zlibCompress(QByteArray in)
{
    QByteArray result;

    uLongf destLength = compressBound(in.size());

    result.resize(destLength);

    compress2((Bytef *) result.data(), &destLength, (Bytef *)in.data(), in.size(), Z_BEST_COMPRESSION);

    result.resize(destLength);

    return result;
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
