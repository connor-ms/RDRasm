#include "script.h"

#include <QMessageBox>
#include <QSysInfo>

#include "../util/endianness.h"
#include "../../lib/Qt-AES/qaesencryption.h"

Script::Script(QString path)
    : m_Script(path)
{
    if (!m_Script.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(nullptr, "Error", "Error: Unable to open script.");
        return;
    }

    m_Data = m_Script.readAll();

    ReadHeader();
}

bool Script::ReadHeader()
{
    // Read data from header, which is uncompressed and unencrypted

    QDataStream stream(m_Data);

    // Not sure if necessary, but just being safe
    stream.setByteOrder(Endianness::getEndianness());

    stream >> m_Header.magic;

    stream >> m_Header.version;
    stream >> m_Header.flags1;
    stream >> m_Header.flags2;

    m_Header.vsize = (int)(m_Header.flags2 & 0x7FFF);
    m_Header.psize = (int)((m_Header.flags2 & 0xFFF7000) >> 14);
    m_Header._f14_30 = (int)(m_Header.flags2 & 0x70000000);
    m_Header.extended = (m_Header.flags2 & 0x80000000) == 0x80000000 ? true : false;

    return true;
}
