#include "disassembler.h"
#include "ui_disassembler.h"

#include "../QHexView/qhexview.h"
#include "../QHexView/document/buffer/qmemoryrefbuffer.h"
#include "../rage/script.h"

Disassembler::Disassembler(QString file, QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::Disassembler)
{
    m_ui->setupUi(this);

    m_ui->funcTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    Script script(file);

    fillFuncTable(script.getFunctions());

    //QHexDocument *document = QHexDocument::fromMemory<QMemoryRefBuffer>(script.getData());

   // QHexView *view = new QHexView(this);
    //view->setDocument(document);

    //m_ui->tabWidget->addTab(view, "Hex View");
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

        m_ui->funcTable->setItem(index, 0, new QTableWidgetItem(op->getFormattedSize()));
        m_ui->funcTable->setItem(index, 1, new QTableWidgetItem(QString(op->getData().remove(0, 4))));

        qDebug() << op->getData() << " " << QString(op->getData());
    }

    m_ui->funcTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
}
