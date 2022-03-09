#ifndef OPCODETABLE_H
#define OPCODETABLE_H

#include <QTableWidget>

class OpcodeTable : public QTableWidget
{
public:
    OpcodeTable(int rows, int columns, QWidget *parent);

public slots:
    void displayContextMenu(const QPoint &point);
};

#endif // OPCODETABLE_H
