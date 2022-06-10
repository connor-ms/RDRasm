#ifndef OPCODETABLE_H
#define OPCODETABLE_H

#include <QTableWidget>
#include <QVector>

#include "../rage/iopcode.h"

class OpcodeTable : public QTableWidget
{
public:
    OpcodeTable(int rows, int columns, QWidget *parent);

    void setOpcodes(const QVector<std::shared_ptr<IOpcode>> &ops) { m_ops = ops; }
    void setRowColor(int row, QColor col);

public slots:
    void displayContextMenu(const QPoint &point);

private:
    int openEditDialog(std::shared_ptr<IOpcode> op);

    QVector<std::shared_ptr<IOpcode>> m_ops;

    QMenu *menu;
    QAction *edit;
};

#endif // OPCODETABLE_H
