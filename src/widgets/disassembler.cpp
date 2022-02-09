#include "disassembler.h"
#include "ui_disassembler.h"

#include "../rage/script.h"

Disassembler::Disassembler(QString file, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Disassembler)
{
    ui->setupUi(this);

    Script script(file);
}

Disassembler::~Disassembler()
{
    delete ui;
}
