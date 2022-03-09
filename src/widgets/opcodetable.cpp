#include "opcodetable.h"

#include <QHeaderView>
#include <QMenu>

#include <QDebug>

OpcodeTable::OpcodeTable(int rows, int columns, QWidget *parent)
    : QTableWidget(rows, columns, parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &QTableWidget::customContextMenuRequested, this, &OpcodeTable::displayContextMenu);
}

void OpcodeTable::displayContextMenu(const QPoint &point)
{
    QTableWidgetItem *item = itemAt(point);

    if (item == nullptr)
        return;

    QMenu *menu = new QMenu(this);

    menu->addAction(item->text());

    menu->exec(horizontalHeader()->viewport()->mapToGlobal(point));
}
