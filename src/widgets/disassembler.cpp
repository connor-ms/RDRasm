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

    Script script(file);

    QHexDocument *document = QHexDocument::fromMemory<QMemoryRefBuffer>(script.getData());

    QHexView *view = new QHexView(this);
    view->setDocument(document);

    m_ui->tabWidget->addTab(view, "Hex View");
}

Disassembler::~Disassembler()
{
    delete m_ui;
}
