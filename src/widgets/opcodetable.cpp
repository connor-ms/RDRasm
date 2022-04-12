#include "opcodetable.h"

#include <QHeaderView>
#include <QMenu>

OpcodeTable::OpcodeTable(int rows, int columns, QWidget *parent)
    : QTableWidget(rows, columns, parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setFont(QFont("Roboto Mono", 10));
    setHorizontalHeaderLabels({ "Address", "Bytes", "Opcode", "Data" });
    setColumnWidth(0, 125);
    setShowGrid(false);

    horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Fixed);

    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(10);
    verticalHeader()->setVisible(false);

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
