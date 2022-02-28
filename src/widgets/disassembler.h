#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <QMainWindow>

#include <memory>

#include "../rage/opcodefactory.h"

namespace Ui {
class Disassembler;
}

class Disassembler : public QMainWindow
{
    Q_OBJECT

public:
    explicit Disassembler(QString file, QWidget *parent = nullptr);
    ~Disassembler();

    void fillFuncTable(std::vector<std::shared_ptr<IOpcode>> funcs);

private:
    Ui::Disassembler *m_ui;
};

#endif // DISASSEMBLER_H
