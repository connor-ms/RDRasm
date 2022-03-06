#include "util.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "lzx.h"

QByteArray Util::getAESKey()
{
    QFile key("C:\\users\\csull\\Desktop\\rdr_key.bin");

    if (!key.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(nullptr, "Error", "Error: unable to retrieve AES key. Make sure 'key.bin' exists in the root directory.");
        return QByteArray();
    }

    return key.readAll();
}


void Util::decompressBuffer(unsigned char *in, unsigned char *out, int outsize)
{
    struct LZXstate *lzx_state = lzxInit(17);

    int outputSize = 0;

    int offset = 0;

    while (outputSize != outsize)
    {
        int tmpoutputSize = 0;
        int tmpinputSize = 0;

        if (*(unsigned char *)(in + offset) == 0xff)
        {
            tmpoutputSize = *(unsigned char *)(in + offset + 1) << 8;
            tmpoutputSize |= *(unsigned char *)(in + offset + 2);
            tmpinputSize = *(unsigned char *)(in + offset + 3) << 8;
            tmpinputSize |= *(unsigned char *)(in + offset + 4);

            offset += 5;
        }
        else
        {
            tmpoutputSize = 0x8000;
            tmpinputSize = (*(unsigned char *)(in + offset) << 8) | *(unsigned char *)(in + offset + 1);
            if (tmpinputSize == 0)
                break;
            offset += 2;
        }

        int res = lzxDecompress(lzx_state, in + offset, out + outputSize, tmpinputSize, tmpoutputSize);
        if (res != 0)
        {
            QMessageBox::critical(0, "Error", QString("Error: LZX decompression failed! (Error code: %1)").arg(res));
            memset(out, 0, outsize);
            break;
        }

        offset += tmpinputSize;
        outputSize += tmpoutputSize;
    }

    lzxTeardown(lzx_state);
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
