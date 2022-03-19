#include "disassembler.h"
#include "ui_disassembler.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "../util/util.h"
#include "../QHexView/qhexview.h"
#include "../QHexView/document/buffer/qmemoryrefbuffer.h"

Disassembler::Disassembler(QString file, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::Disassembler)
    , m_script(file)
    , m_file(file)
{
    m_ui->setupUi(this);

    m_ui->funcTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_nativeMap = Util::getNatives();

    fillFuncTable(m_script.getFunctions());
    createDisassemblyTab();
    createStringsTab();
    createScriptDataTab();

    connect(m_ui->actionExportDisassembly, SIGNAL(triggered()), this, SLOT(exportDisassembly()));
    connect(m_ui->actionExportRawData,     SIGNAL(triggered()), this, SLOT(exportRawData()));
}

Disassembler::~Disassembler()
{
    delete m_ui;
}

void Disassembler::exportDisassembly()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Export disassembly", m_file.split("\\").last().split(".xsc").first(), "Text (*.txt)");
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "Error", "Error: unable to write to file. Make sure the file isn't open elsewhere.");
        return;
    }

    QString str;

    for (int row = 0; row < m_disasm->rowCount(); row++)
    {
        for (int col = 0; col < m_disasm->columnCount(); col++)
        {
            QTableWidgetItem *item = m_disasm->item(row, col);

            if (item != nullptr)
            {
                str += m_disasm->item(row, col)->text().leftJustified(15);
            }
        }

        str += "\n";
    }

    QTextStream stream(&file);

    stream << str;

    file.close();
}

void Disassembler::createScriptDataTab()
{
    QTextEdit *scriptData = new QTextEdit(this);

    scriptData->setFont(QFont("Courier", 10));

    m_ui->tabWidget->addTab(scriptData, "Script Data");

    scriptData->append(getResourceHeaderData());
    scriptData->append(getScriptHeaderData());
}

void Disassembler::exportRawData()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Export raw data", m_file.split("\\").last().split(".xsc").first(), "Binary data (*.bin)");
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "Error", "Error: unable to write to file. Make sure the file isn't open elsewhere.");
        return;
    }

    file.write(m_script.getData());

    file.close();
}

QString Disassembler::getResourceHeaderData()
{
    QString resourceData;

    resourceData.append("Resource data:\n");
    resourceData.append(QString("    magic: %1\n").arg(m_script.getResourceHeader().magic));
    resourceData.append(QString("    version: %1\n").arg(m_script.getResourceHeader().version));
    resourceData.append(QString("    flags1: %1\n").arg(m_script.getResourceHeader().flags1));
    resourceData.append(QString("    flags2: %1\n").arg(m_script.getResourceHeader().flags2));
    resourceData.append(QString("    vsize: %1\n").arg(m_script.getResourceHeader().vSize));
    resourceData.append(QString("    psize: %1\n").arg(m_script.getResourceHeader().pSize));
    resourceData.append(QString("    _f14_30: %1\n").arg(m_script.getResourceHeader()._f14_30));
    resourceData.append(QString("    extended: %1\n").arg(m_script.getResourceHeader().extended));
    resourceData.append(QString("    virtual size: %1\n").arg(m_script.getResourceHeader().getSizeV()));
    resourceData.append(QString("    physical size: %1\n\n").arg(m_script.getResourceHeader().getSizeP()));

    return resourceData;
}

QString Disassembler::getScriptHeaderData()
{
    QString scriptData;

    scriptData.append("Script data:\n");
    scriptData.append(QString("    headerPos: %1\n").arg(m_script.getScriptHeader().headerPos));
    scriptData.append(QString("    magic: %1\n").arg(m_script.getScriptHeader().magic));
    scriptData.append(QString("    pageMapPtr: %1\n").arg(m_script.getScriptHeader().pageMapPtr));
    scriptData.append(QString("    codeSize: %1\n").arg(m_script.getScriptHeader().codeSize));
    scriptData.append(QString("    paramCount: %1\n").arg(m_script.getScriptHeader().paramCount));
    scriptData.append(QString("    staticsSize: %1\n").arg(m_script.getScriptHeader().staticsSize));
    scriptData.append(QString("    staticsPtr: %1\n").arg(m_script.getScriptHeader().staticsPtr));
    scriptData.append(QString("    globalsVers: %1\n").arg(m_script.getScriptHeader().globalsVers));
    scriptData.append(QString("    nativesSize: %1\n").arg(m_script.getScriptHeader().nativesSize));
    scriptData.append(QString("    nativesPtr: %1\n").arg(m_script.getScriptHeader().nativesPtr));
    scriptData.append(QString("    codePagesSize: %1\n").arg(m_script.getScriptHeader().codePagesSize));
    scriptData.append(QString("    codePagesPtr: %1\n").arg(m_script.getScriptHeader().codePagesPtr));

    return scriptData;
}

void Disassembler::fillFuncTable(std::vector<std::shared_ptr<IOpcode>> funcs)
{
    int funcCount = 0;

    for (auto op : funcs)
    {
        int index = m_ui->funcTable->rowCount();

        m_ui->funcTable->setRowCount(index + 1);

        m_ui->funcTable->setItem(index, 0, new QTableWidgetItem(op->getFormattedLocation()));

        QByteArray data = op->getData().remove(0, 4);

        if (data.isEmpty())
        {
            if (funcCount == 0)
            {
                m_ui->funcTable->setItem(index, 1, new QTableWidgetItem(QString("__entrypoint")));
            }
            else
            {
                m_ui->funcTable->setItem(index, 1, new QTableWidgetItem(QString("func_%1").arg(funcCount)));
            }
        }
        else
        {
            m_ui->funcTable->setItem(index, 1, new QTableWidgetItem(QString(data)));
        }

        funcCount++;
    }

    m_ui->funcTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
}

void Disassembler::createDisassemblyTab()
{
    m_disasm = new OpcodeTable(0, 4, m_ui->tabWidget);

    m_disasm->setFont(QFont("Courier", 10));
    m_disasm->setHorizontalHeaderLabels({ "Address", "Bytes", "Opcode", "Data" });
    m_disasm->setColumnWidth(0, 125);
    m_disasm->setShowGrid(false);

    m_disasm->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
    m_disasm->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Fixed);

    QHeaderView *verticalHeader = m_disasm->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(10);
    verticalHeader->setVisible(false);

    m_ui->tabWidget->addTab(m_disasm, "Disassembly");

    fillDisassembly();
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

void Disassembler::fillDisassembly()
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

        int index = m_disasm->rowCount();
        m_disasm->setRowCount(index + 1);

        if (m_script.getJumps().count(op->getLocation()) == 1)
        {
            QTableWidgetItem *jump = new QTableWidgetItem(QString(":" + m_script.getJumps().at(op->getLocation())));
            jump->setForeground(QColor(255, 0, 0));

            m_disasm->setItem(index++, 1, jump);
            m_disasm->insertRow(index);
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
                m_disasm->insertRow(index++);
            }
        }
        else if (op->getOp() >= EOpcodes::OP_JMP && op->getOp() <= EOpcodes::OP_JMPGT)
        {
            int jumpPos = op->getData()[1] + op->getLocation() + 3;
            data->setText("@" + m_script.getJumps().at(jumpPos));
            data->setForeground(QColor(255, 0, 0));
        }

        m_disasm->setItem(index, 0, address);
        m_disasm->setItem(index, 1, bytes);
        m_disasm->setItem(index, 2, opcode);
        m_disasm->setItem(index, 3, data);
    }
}
