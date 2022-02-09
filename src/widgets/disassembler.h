#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <QMainWindow>

namespace Ui {
class Disassembler;
}

class Disassembler : public QMainWindow
{
    Q_OBJECT

public:
    explicit Disassembler(QString file, QWidget *parent = nullptr);
    ~Disassembler();

private:
    Ui::Disassembler *ui;
};

#endif // DISASSEMBLER_H
