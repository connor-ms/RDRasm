#include "util.h"

#include <QFile>
#include <QMessageBox>

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
