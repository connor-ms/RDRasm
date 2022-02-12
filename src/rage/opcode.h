#ifndef OPCODE_H
#define OPCODE_H

#include "opcodebase.h"

class Opcode : public OpcodeBase
{
public:
    Opcode(OpcodeBase base);

    QString string();

    int getSize();

    QByteArray getHex();
    int getOffset();

private:
    QByteArray m_hex;
    unsigned int m_offset;

    int m_size;

    Opcode *m_next;
    Opcode *m_prev;
};

#endif // OPCODE_H
