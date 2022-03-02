#include "disassembler.h"
#include "ui_disassembler.h"

#include "../QHexView/qhexview.h"
#include "../QHexView/document/buffer/qmemoryrefbuffer.h"

Disassembler::Disassembler(QString file, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::Disassembler)
    , m_script(file)
{
    m_ui->setupUi(this);

    m_ui->funcTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    fillFuncTable(m_script.getFunctions());

    QTextEdit *disasm = new QTextEdit(this);

    disasm->setWordWrapMode(QTextOption::NoWrap);
    disasm->setFont(QFont("Consolas", 10));

    m_ui->tabWidget->addTab(disasm, "Disassembly");

    fillDisassembly(disasm);

    disasm->moveCursor(QTextCursor::Start);
}

Disassembler::~Disassembler()
{
    delete m_ui;
}

void Disassembler::fillFuncTable(std::vector<std::shared_ptr<IOpcode>> funcs)
{
    for (auto op : funcs)
    {
        int index = m_ui->funcTable->rowCount();

        m_ui->funcTable->setRowCount(index + 1);

        m_ui->funcTable->setItem(index, 0, new QTableWidgetItem(op->getFormattedLocation()));
        m_ui->funcTable->setItem(index, 1, new QTableWidgetItem(QString(op->getData().remove(0, 4))));
    }

    m_ui->funcTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
}

void Disassembler::fillDisassembly(QTextEdit *textEdit)
{
    int funcCount = 0;

    for (auto op : m_script.getOpcodes())
    {
        textEdit->moveCursor(QTextCursor::End);

        if (op->getOp() == EOpcodes::OP_ENTER)
        {
            QString funcName;

            if (op->getArgsString().isEmpty() && funcCount > 0)
            {
                funcName = QString("func_%1").arg(funcCount);
            }
            else if (funcCount == 0)
            {
                funcName = "__entrypoint";
            }
            else
            {
                funcName = op->getArgsString();
            }

            textEdit->insertPlainText(QString("%1%2   %3 (%4 args):\n").arg(funcCount == 0 ? "" : "\n").arg(op->getFormattedLocation()).arg(funcName).arg(QString::number(op->getData()[0])));

            funcCount++;
        }
        else
        {
            textEdit->insertPlainText(op->getString() + "\n");
        }
    }
}
