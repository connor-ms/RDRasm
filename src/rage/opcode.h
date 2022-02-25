#ifndef OPCODE_H
#define OPCODE_H

#include <QByteArray>
#include <QDataStream>

class Opcode
{
public:
    Opcode(QString name, int id, int size);
    Opcode(Opcode &other);

    void read(QDataStream *stream);

private:
    QString m_name;
    int m_id;
    int m_size;

    QByteArray m_data;
};

#endif // OPCODE_H
