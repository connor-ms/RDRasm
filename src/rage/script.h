#ifndef SCRIPT_H
#define SCRIPT_H

#include <QFile>
#include <QString>

#include "opcode.h"

struct ResourceHeader
{
    int magic;
    int version;
    int flags1;
    int flags2;

    int vSize;
    int pSze;
    int _f14_30;
    bool extended;

    int getSizeV() { return extended ? (vSize << 12) : ((int)(flags1 & 0x7FF) << ((int)((flags1 >> 11) & 15) + 8)); }
    int getSizeP() { return extended ? (pSze << 12) : ((int)((flags1 >> 15) & 0x7FF) << ((int)((flags1 >> 26) & 15) + 8)); }
};

struct ScriptHeader
{
    int magic;
    int pageMapPtr;
    int codeSize;
    int paramCount;

    int staticsSize;
    int staticsPtr;

    int globalsVers;
    int nativesSize;
    int nativesPtr;

    int codePagesSize;
    int codePagesPtr;
    std::vector<int> codePages;
};

class Script
{
public:
    Script(QString path);

    QByteArray getData();

    static QList<OpcodeBase> getOpcodes();

private:
    void readRSCHeader();
    void extractData();
    int findScriptHeader();
    void readScriptHeader(int headerPos);

    ResourceHeader m_header;
    ScriptHeader m_scriptHeader;

    QByteArray m_data;

    QFile m_script;
};

#endif // SCRIPT_H
