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
    createDisassemblyTab();
    createStringsTab();
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

QTextEdit *Disassembler::createDisassemblyTab()
{
    QTextEdit *disasm = new QTextEdit(this);

    disasm->setWordWrapMode(QTextOption::NoWrap);
    disasm->setFont(QFont("Consolas", 10));

    m_ui->tabWidget->addTab(disasm, "Disassembly");

    fillDisassembly(disasm);

    disasm->moveCursor(QTextCursor::Start);

    return disasm;
}

QTableWidget *Disassembler::createStringsTab()
{
    QTableWidget *stringTable = new QTableWidget(m_script.getStrings().size(), 2, this);

    stringTable->setHorizontalHeaderLabels({"Location", "String"});
    stringTable->verticalHeader()->setVisible(false);
    stringTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);

    for (unsigned int i = 0; i < m_script.getStrings().size(); i++)
    {
        auto op = m_script.getStrings()[i];

        stringTable->setItem(i, 0, new QTableWidgetItem(op->getFormattedLocation()));
        stringTable->setItem(i, 1, new QTableWidgetItem(op->getArgsString()));
    }

    m_ui->tabWidget->addTab(stringTable, "Strings");

    return stringTable;
}

void Disassembler::fillDisassembly(QTextEdit *textEdit)
{
    int funcCount = 0;

    for (auto op : m_script.getOpcodes())
    {
        if (op->getOp() == EOpcodes::OP_NOP)
            continue;

        textEdit->moveCursor(QTextCursor::End);

        if (m_script.getJumps().count(op->getLocation()) == 1)
        {
            textEdit->insertPlainText(QString(m_script.getJumps().at(op->getLocation()) + '\n').rightJustified(26));
        }

        if (op->getOp() == EOpcodes::OP_ENTER)
        {
            QString funcName;

            if (op->getArgsString().isEmpty() && funcCount > 0)
            {
                funcName = "func_" + QString::number(funcCount).rightJustified(5, '0');
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
        else if (op->getOp() >= EOpcodes::OP_JMP && op->getOp() <= EOpcodes::OP_JMPGT)
        {
            int jumpPos = op->getData()[1] + op->getLocation() + 3;
            textEdit->insertPlainText(op->getFormattedLocation() + "   " + op->getDataString().leftJustified(10, ' ') + "   " + op->getName().leftJustified(10) + "   " + m_script.getJumps().at(jumpPos) + '\n');
        }
        else
        {
            textEdit->insertPlainText(op->getString() + "\n");
        }
    }
}
