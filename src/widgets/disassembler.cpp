#include "disassembler.h"
#include "ui_disassembler.h"

#include "../util/util.h"
#include "../QHexView/qhexview.h"
#include "../QHexView/document/buffer/qmemoryrefbuffer.h"

Disassembler::Disassembler(QString file, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::Disassembler)
    , m_script(file)
{
    m_ui->setupUi(this);

    m_ui->funcTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_nativeMap = Util::getNatives();

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

OpcodeTable *Disassembler::createDisassemblyTab()
{
    OpcodeTable *disasm = new OpcodeTable(0, 4, m_ui->tabWidget);

    disasm->setFont(QFont("Courier", 10));
    disasm->setHorizontalHeaderLabels({ "Address", "Bytes", "Opcode", "Data" });
    disasm->setColumnWidth(0, 125);
    disasm->setShowGrid(false);

    disasm->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
    disasm->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Fixed);

    QHeaderView *verticalHeader = disasm->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(10);
    verticalHeader->setVisible(false);

    m_ui->tabWidget->addTab(disasm, "Disassembly");

    fillDisassembly(disasm);

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

void Disassembler::fillDisassembly(QTableWidget *disasm)
{
    int funcCount = 0;

    for (auto op : m_script.getOpcodes())
    {
        if (op->getOp() == EOpcodes::OP_NOP)
            continue;

        QTableWidgetItem *address = new QTableWidgetItem(op->getFormattedLocation());
        QTableWidgetItem *bytes   = new QTableWidgetItem(op->getDataString());
        QTableWidgetItem *opcode  = new QTableWidgetItem(op->getName());
        QTableWidgetItem *data    = new QTableWidgetItem(op->getArgsString());

        opcode->setForeground(QColor(48,  98, 174));
        bytes->setForeground(QColor(120, 120, 120));

        int index = disasm->rowCount();
        disasm->setRowCount(index + 1);

        if (m_script.getJumps().count(op->getLocation()) == 1)
        {
            QTableWidgetItem *jump = new QTableWidgetItem(QString(":" + m_script.getJumps().at(op->getLocation())));
            jump->setForeground(QColor(255, 0, 0));

            disasm->setItem(index++, 1, jump);
            disasm->insertRow(index);
        }

        if (op->getOp() == EOpcodes::OP_NATIVE)
        {
            int native   = ((op->getData()[0] << 2) & 0x300) | op->getData()[1];
            int argCount = (op->getData()[0] & 0x3e) >> 1;
            bool hasRets = (op->getData()[0] & 1) == 1 ? true : false;

            data->setText(QString("%1 (%2 args, ret %3)").arg(Util::getNative(m_script.getNatives()[native], m_nativeMap))
                                                         .arg(argCount)
                                                         .arg(hasRets));

        }
        else if (op->getOp() == EOpcodes::OP_ENTER)
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

            funcCount++;

            if (funcCount != 1)
            {
                disasm->insertRow(index++);
            }
        }
        else if (op->getOp() >= EOpcodes::OP_JMP && op->getOp() <= EOpcodes::OP_JMPGT)
        {
            int jumpPos = op->getData()[1] + op->getLocation() + 3;
            data->setText("@" + m_script.getJumps().at(jumpPos));
            data->setForeground(QColor(255, 0, 0));
        }

        disasm->setItem(index, 0, address);
        disasm->setItem(index, 1, bytes);
        disasm->setItem(index, 2, opcode);
        disasm->setItem(index, 3, data);
    }
}
