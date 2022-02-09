#ifndef SCRIPT_H
#define SCRIPT_H

#include <QFile>
#include <QString>

struct ScriptHeader
{
    int magic;
    int version;
    int flags1;
    int flags2;

    int vsize;
    int psize;
    int _f14_30;
    bool extended;

    int GetSizeV() { return extended ? (vsize << 12) : ((int)(flags1 & 0x7FF) << ((int)((flags1 >> 11) & 15) + 8)); }
    int GetSizeP() { return extended ? (psize << 12) : ((int)((flags1 >> 15) & 0x7FF) << ((int)((flags1 >> 26) & 15) + 8)); }
};

class Script
{
public:
    Script(QString path);

private:
    bool ReadHeader();

    ScriptHeader m_Header;

    QFile m_Script;
    QByteArray m_Data;
};

#endif // SCRIPT_H
