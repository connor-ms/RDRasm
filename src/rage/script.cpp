#include "script.h"

#include <QMessageBox>
#include <QSysInfo>

#include "../rage/opcodefactory.h"
#include "../util/util.h"

#define ReadVar(x) stream >> x;
#define ReadPointer(x) stream >> x; x = x & 0xffffff;

Script::Script(QString path)
    : m_funcCount(0)
    , m_script(path)
{
    if (!m_script.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(nullptr, "Error", "Error: Unable to open script.");
        return;
    }

    m_scriptHeader.headerPos = -1;

    if (path.contains(".csc"))
        m_scriptType = ScriptType::TYPE_PS3;
    else
        m_scriptType = ScriptType::TYPE_X360;

    m_data = m_script.readAll();

    // Decompress and unencrypt script from inside resource file
    if (readRSCHeader() == false)
    {
        QMessageBox::critical(nullptr, "Error", "Error: Invalid script.");

        return;
    }

    extractData();

    // Begin disassembling script once extracted from resource file
    m_scriptHeader.headerPos = findScriptHeader();

    if (m_scriptHeader.headerPos == -1)
    {
        QMessageBox::critical(nullptr, "Error", "Error: Unable to find script header.");
        return;
    }

    readScriptHeader(m_scriptHeader.headerPos);
    readNatives();
    readPages();
}

bool Script::readRSCHeader()
{
    // Read data from header, which doesn't need to be uncompressed or unencrypted

    QDataStream stream(m_data);

    stream >> m_header.magic;

    if (m_header.magic != 0x85435352 && m_header.magic != 0x86435352)
    {
        return false;
    }

    stream >> m_header.version;
    stream >> m_header.flags1;
    stream >> m_header.flags2;

    m_header.vSize    = (int)(m_header.flags2 & 0x7FFF);
    m_header.pSize    = (int)((m_header.flags2 & 0xFFF7000) >> 14);
    m_header._f14_30  = (int)(m_header.flags2 & 0x70000000);
    m_header.extended = (m_header.flags2 & 0x80000000) == 0x80000000 ? true : false;

    return true;
}

void Script::extractData()
{
    m_footer = m_data.mid(m_data.size() - (m_data.size() % 16));

    if (m_header.version == 2)
    {
        // remove rsc header
        m_data = m_data.remove(0, 16);

        m_data = Util::decrypt(m_data);

        int outSize = m_header.getSizeP() + m_header.getSizeV();

        if (m_scriptType == ScriptType::TYPE_X360)
        {
            // remove lzx header
            m_data.remove(0, 8);

            m_data = Util::lzxDecompress(m_data, outSize);
        }
        else
        {
            m_data = Util::zlibDecompress(m_data, outSize);
        }
    }
}

int Script::findScriptHeader()
{
    int headerOffset = 0;
    bool found = false;

    QDataStream stream(m_data);

    while (headerOffset <= m_data.size())
    {
        int magic;
        stream >> magic;

        if (magic == -1462287616)
        {
            found = true;
            break;
        }

        headerOffset += 4096;
        stream.device()->seek(headerOffset);
    }

    // return -1 if header wasn't found
    return found ? headerOffset : -1;
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

    int prevEnd = 0;

    // get address for each page and read it
    for (int i = 0; i < m_scriptHeader.codePagesSize; i++)
    {
        int address;
        ReadPointer(address);

        m_pageOffsets.push_back(address - prevEnd);
        m_pageLocations.push_back(address);

        prevEnd = address + 0x4000;

        readPage(address, i);
    }
}

unsigned int Script::getPageByLocation(unsigned int location)
{
    for (unsigned int i = 0; i < m_pageLocations.size(); i++)
    {
        if (m_pageLocations[i] <= location && m_pageLocations[i] + 0x4000 >= location)
        {
            return i;
        }
    }

    return 0;
}

void Script::readPage(int address, int page)
{
    QDataStream stream(m_data);

    stream.device()->seek(address);

    // set length to 0x4000, unless last page, then set length to remainder
    int length = (page == m_scriptHeader.codePagesSize - 1) ? m_scriptHeader.codeSize % 0x4000 : 0x4000;
    int jumpCount = 0;

    while (stream.device()->pos() < address + length)
    {
        unsigned char opcode;
        ReadVar(opcode);

        auto op = OpcodeFactory::Create((EOpcodes)opcode);

        op->read(&stream);
        op->setPage(page);

        if (op->getOp() == EOpcodes::OP_ENTER)
        {
            QString funcName;

            if (op->getFormattedData().isEmpty() && m_funcCount > 0)
            {
                funcName = "func_" + QString::number(m_funcCount).rightJustified(5, '0');
            }
            else if (m_funcCount == 0)
            {
                funcName = "__entrypoint";
            }
            else
            {
                funcName = op->getFormattedData();
            }

            if (m_funcNames.count(op->getLocation()) == 0)
            {
                int offset = op->getLocation();
                m_funcNames.insert(std::pair<unsigned int, QString>(offset, funcName));
            }

            m_funcCount++;
        }
        else if (op->getOp() == EOpcodes::OP_SPUSH || op->getOp() == EOpcodes::OP_SPUSHL)
        {
            m_strings.push_back(op);
        }
        else if (op->getOp() >= EOpcodes::OP_JMP && op->getOp() <= EOpcodes::OP_JMPGT)
        {
            int jumpPos = op->getData()[1] + op->getLocation() + 3;

            m_jumps.insert(std::pair<int, QString>(jumpPos, "sub_" + QString::number(jumpCount).rightJustified(5, '0')));
            jumpCount++;
        }

        m_opcodes.push_back(op);
    }
}

void Script::rebuild()
{
    for (unsigned int page = 0; page < getPageOffsets().size(); page++)
    {
        unsigned int pos = getPageLocations()[page];
        unsigned int length = (page == (unsigned int)m_scriptHeader.codePagesSize - 1) ? m_scriptHeader.codeSize % 0x4000 : 0x4000;

        // clear current page
        memset(m_data.data() + pos, 0, length);

        std::list<std::shared_ptr<IOpcode>>::iterator itr = m_opcodes.begin();

        while (pos < getPageLocations()[page] + length)
        {
            if (itr == m_opcodes.end())
                break;

            std::shared_ptr<IOpcode> op = *itr;

            int size = op->getData().size() + 1;

            memcpy(m_data.data() + pos, op->getFullData().data(), size);

            pos += size;
            std::advance(itr, 1);
        }
    }
}
