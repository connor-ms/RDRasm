#include "opcodetable.h"

#include <QHeaderView>
#include <QMenu>

#include "editdialog.h"

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

void OpcodeTable::setRowColor(int row, QColor col)
{
    for (int i = 0; i < 4; i++)
    {
        item(row, i)->setBackground(col);
    }
}

int OpcodeTable::openEditDialog(std::shared_ptr<IOpcode> op)
{
    EditDialog *dialog = new EditDialog(op);

    return dialog->exec();
}

void OpcodeTable::displayContextMenu(const QPoint &point)
{
    QTableWidgetItem *selected = itemAt(point);

    if (selected == nullptr)
        return;

    QMenu *menu = new QMenu(this);

    auto op = m_ops.at(selected->row() + 1);

    int dialogResult = 0;

    if (op->getDeleted())
    {
        menu->addAction("Undelete", [this, selected, op]{ op->setDeleted(false); setRowColor(selected->row(), QColor(255, 255, 255)); });
    }
    else
    {
        menu->addAction("Edit", [this, op, &dialogResult]{ dialogResult = openEditDialog(op); });
        menu->addAction("Delete", [this, selected, op]{ op->setDeleted(true); setRowColor(selected->row(), QColor(255, 0, 0)); });
    }

    menu->exec(horizontalHeader()->viewport()->mapToGlobal(QPoint(point.x(), point.y() + verticalHeader()->sectionSize(0))));

    if (dialogResult == QDialog::Accepted)
    {
        for (int i = 0; i < 4; i++)
        {
            setRowColor(selected->row(), QColor(0, 255, 0));
        }

        item(selected->row(), 3)->setText(op->getFormattedData());
    }
}
