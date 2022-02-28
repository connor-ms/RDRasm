#ifndef STRING_H
#define STRING_H

#include "../opcodefactory.h"

MAKE_SIMPLE_OP(Op_SPush0, EOpcodes::OP_SPUSH0, "spush0", 1); // push ""
MAKE_SIMPLE_OP(Op_SCpy,   EOpcodes::OP_SCPY,   "scpy",   2);
MAKE_SIMPLE_OP(Op_SAdd,   EOpcodes::OP_SADD,   "sadd",   2);
MAKE_SIMPLE_OP(Op_SAddI,  EOpcodes::OP_SADDI,  "saddi",  2);
MAKE_SIMPLE_OP(Op_SNCpy,  EOpcodes::OP_SNCPY,  "sncpy",  1);

class Op_SPush : public IOpcode, public RegisteredInFactory<Op_SPush>
{
    REGISTER(Op_SPush, EOpcodes::OP_SPUSH, "spush")
public:
    virtual void read(QDataStream *stream) override;
};

class Op_SPushL : public IOpcode, public RegisteredInFactory<Op_SPushL>
{
    REGISTER(Op_SPushL, EOpcodes::OP_SPUSHL, "spuhsl")
public:
    virtual void read(QDataStream *stream) override;
};
//OP_SPUSH,
//OP_SPUSHL,

//OP_REGISTER(OP_SPush);

#endif // STRING_H
