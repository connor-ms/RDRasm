#ifndef COMPILER_H
#define COMPILER_H

#include <QVector>

#include "script.h"
#include "iopcode.h"
#include "opcodes/enter.h"
#include "opcodes/helper.h"

struct WriteSize
{
    WriteSize(QString name, unsigned int size) { m_name = name; m_size = size; m_written = false; }
    QString m_name;
    unsigned int m_size;
    bool m_written;
};

class Compiler
{
public:
    Compiler(Script &script);

    QByteArray compile();

private:
    int roundUp(int value, int round);
    int roundDown(int value, int round);

    void copy();
    void clean();

    int findHeaderPos(std::vector<unsigned int> pageSizes);
    std::vector<unsigned int> getDataPageSizes(int codeSize);
    void findWriteOrder(std::vector<WriteSize*> *order);

    void writeHeader();
    int writeCode();
    int writePageMap();
    int writeCodePageMap();
    int writeNatives();
    int writeStatics();

    QVector<std::shared_ptr<IOpcode>> m_code;
    QVector<int> m_codePageOffsets;

    QVector<int> m_statics;
    QVector<unsigned int> m_natives;

    QVector<std::shared_ptr<Op_Enter>> m_funcs;
    QVector<std::shared_ptr<Op_HSub>>  m_subs;

    int m_pageCount;

    Script *m_origScript;
    ScriptHeader m_header;

    QByteArray m_result;
};

#endif // COMPILER_H
