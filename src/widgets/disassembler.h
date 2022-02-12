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
    Ui::Disassembler *m_ui;
};

#endif // DISASSEMBLER_H
