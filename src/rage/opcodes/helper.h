#ifndef HELPER_H
#define HELPER_H

#include "../opcodefactory.h"

class Op_HSub : public IOpcode, public RegisteredInFactory<Op_HSub>
{
    REGISTER(Op_HSub, EOpcodes::_SUB, "sub")
public:
    virtual void read(QDataStream *)    override {}; // these should never be called
    virtual QString getFormattedBytes() override { return QString();  }
    virtual QString getFormattedData()  override { return QString();  }
    virtual int getSize()               override { return 0;          }

    QVector<std::shared_ptr<IOpcode>> getReferences() { return m_references;      }
    void addReference(std::shared_ptr<IOpcode> ref)   { m_references.append(ref); }

    void setSub(int sub) { m_sub = sub; }
    QString getSub()     { return QString("sub_%1").arg(m_sub); }

private:
    QVector<std::shared_ptr<IOpcode>> m_references;
    int m_sub;
};

class Op_HSpacer : public IOpcode, public RegisteredInFactory<Op_HSpacer>
{
    REGISTER(Op_HSpacer, EOpcodes::_SPACER, "")
public:
    virtual void read(QDataStream *)    override {}; // these should never be called
    virtual QString getFormattedBytes() override { return QString(); }
    virtual QString getFormattedData()  override { return QString(); }
    virtual int getSize()               override { return 0;         }
};

#endif // HELPER_H
