#include "script.h"

#include <QMessageBox>
#include <QSysInfo>

#include "../rage/opcodefactory.h"
#include "../util/util.h"
#include "../../lib/Qt-AES/qaesencryption.h"

#define ReadVar(x) stream >> x;
#define ReadPointer(x) stream >> x; x = x & 0xffffff;

Script::Script(QString path)
    : m_script(path)
{
    if (!m_script.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(nullptr, "Error", "Error: Unable to open script.");
        return;
    }

    m_data = m_script.readAll();

    // Decompress and unencrypt script from inside resource file
    readRSCHeader();
    extractData();

    // Begin disassembling script once extracted from resource file
    int headerPos = findScriptHeader();

    if (headerPos == 0)
    {
        // TODO: message box with retry option, due to occasional decompression errors
        QMessageBox::critical(nullptr, "Error", "Error: Unable to find script header.");
        return;
    }

    readScriptHeader(headerPos);
    readNatives();
    readPages();
}

void Script::readRSCHeader()
{
    // Read data from header, which doesn't need to be uncompressed or unencrypted

    QDataStream stream(m_data);

    stream >> m_header.magic;

    stream >> m_header.version;
    stream >> m_header.flags1;
    stream >> m_header.flags2;

    m_header.vSize = (int)(m_header.flags2 & 0x7FFF);
    m_header.pSize = (int)((m_header.flags2 & 0xFFF7000) >> 14);
    m_header._f14_30 = (int)(m_header.flags2 & 0x70000000);
    m_header.extended = (m_header.flags2 & 0x80000000) == 0x80000000 ? true : false;
}

void Script::extractData()
{
    if (m_header.version == 2)
    {
        // remove header
        m_data = m_data.remove(0, 16);

        // pad to nearest 16 bytes for AES to be able to decrypt
        m_data.resize(m_data.size() + (16 - (m_data.size() % 16)));

        for (int i = 0; i < 0x10; i++)
        {
            m_data = QAESEncryption::Decrypt(QAESEncryption::Aes::AES_256, QAESEncryption::Mode::ECB, m_data, Util::getAESKey());
        }

        m_data = m_data.remove(0, 8);

        int outsize = m_header.getSizeP() + m_header.getSizeV();

        // TODO: fix memory leak here

        unsigned char *in  = reinterpret_cast<unsigned char *>(m_data.data());
        unsigned char *out = new unsigned char[outsize];

        Util::decompressBuffer(in, out, outsize);

        m_data = QByteArray::fromRawData(reinterpret_cast<char *>(out), outsize);
    }
}

int Script::findScriptHeader()
{
    int headerOffset = 0;

    QDataStream stream(m_data);

    while (headerOffset < m_data.size())
    {
        int magic;
        stream >> magic;

        if (magic == -1462287616)
        {
            break;
        }

        headerOffset += 4096;
        stream.device()->seek(headerOffset);
    }

    return headerOffset;
}

void Script::readScriptHeader(int headerPos)
{
    QDataStream stream(m_data);

    stream.device()->seek(headerPos);

    ReadVar(m_scriptHeader.magic);
    ReadPointer(m_scriptHeader.pageMapPtr);
    ReadPointer(m_scriptHeader.codePagesPtr);
    ReadVar(m_scriptHeader.codeSize);
    ReadVar(m_scriptHeader.paramCount);
    ReadVar(m_scriptHeader.staticsSize);
    ReadPointer(m_scriptHeader.staticsPtr);
    ReadVar(m_scriptHeader.globalsVers);
    ReadVar(m_scriptHeader.nativesSize);
    ReadPointer(m_scriptHeader.nativesPtr);

    // amount of code pages
    m_scriptHeader.codePagesSize = (m_scriptHeader.codeSize + (1 << 14) - 1) >> 14;
}

void Script::readNatives()
{
    QDataStream stream(m_data);

    stream.device()->seek(m_scriptHeader.nativesPtr);

    unsigned int nativeHash;

    for (int i = 0; i < m_scriptHeader.nativesSize; i++)
    {
        ReadVar(nativeHash);
        m_natives.push_back(nativeHash);
    }
}

void Script::readPages()
{
    QDataStream stream(m_data);

    stream.device()->seek(m_scriptHeader.codePagesPtr);

    // get address for each page and read them
    for (int i = 0; i < m_scriptHeader.codePagesSize; i++)
    {
        int address;
        ReadPointer(address);

        readPage(address, i);
    }
}

void Script::readPage(int address, int page)
{
    QDataStream stream(m_data);

    stream.device()->seek(address);

    // set length to 0x4000, unless last page, then set length to remainder
    int length = (page == m_scriptHeader.codePagesSize - 1) ? m_scriptHeader.codeSize % 0x4000 : 0x4000;

    while (stream.device()->pos() < address + length)
    {
        unsigned char opcode;
        ReadVar(opcode);

        auto op = OpcodeFactory::Create((EOpcodes)opcode);

        op->read(&stream);
        op->setPage(page);

        if (op->getOp() == EOpcodes::OP_ENTER)
        {
            m_functions.push_back(op);
        }

        m_opcodes.push_back(op);
    }
}
