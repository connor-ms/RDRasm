#ifndef ENTER_H
#define ENTER_H

#include "../opcodefactory.h"

// Has to be in it's own file to prevent linker errors

class Op_Enter : public IOpcode, public RegisteredInFactory<Op_Enter>
{
    REGISTER(Op_Enter, EOpcodes::OP_ENTER, "enter")
public:
    virtual void read(QDataStream *stream) override;
    virtual QString getFormattedBytes() override;
    virtual QString getFormattedData() override;

    virtual QVector<std::shared_ptr<IOpcode>> getReferences() { return m_references;      }
    virtual void addReference(std::shared_ptr<IOpcode> ref)   { m_references.append(ref); }

    virtual void setFuncName(QString name) { m_funcName = name; }
    virtual QString getFuncName() { return m_funcName; }

private:
    QVector<std::shared_ptr<IOpcode>> m_references;
    QString m_funcName;
};
#endif // ENTER_H
