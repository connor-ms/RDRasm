#ifndef OPCODEBASE_H
#define OPCODEBASE_H

#include <QString>

class OpcodeBase
{
    friend class Opcode; // can't access protected variables otherwise? didn't work otherwise so idk

public:
    OpcodeBase(QString name, int initialSize);

protected:
    QString m_name;
    int m_initialSize;
};

#endif // OPCODEBASE_H
