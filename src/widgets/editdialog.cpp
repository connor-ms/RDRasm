#include "editdialog.h"
#include "ui_editdialog.h"

#include <QMessageBox>

#include "../rage/opcodefactory.h"

#include <QDebug>

EditDialog::EditDialog(std::shared_ptr<IOpcode> op, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::EditDialog),
    m_op(op)
{
    m_ui->setupUi(this);

    m_ui->lblOpName->setText(op->getName());
    m_ui->lblOpName->setFont(QFont("Roboto Mono", 10));

    m_ui->editBox->setFont(QFont("Roboto Mono", 10));

    if (op->getOp() == EOpcodes::OP_SPUSH)
    {
        m_ui->editBox->setText(op->getFormattedData().remove("\""));
    }
    else
    {
        m_ui->editBox->setText(op->getData().toHex());
    }
}

EditDialog::~EditDialog()
{
    delete m_ui;
}

void EditDialog::accept()
{
    QString data = m_ui->editBox->text();
    QByteArray result;

    if (m_op->getOp() == EOpcodes::OP_SPUSH)
    {
        result += data.size() + 1;
        result += data;
        result += '\0';
    }
    else
    {
        result = QByteArray::fromHex(data.toLatin1());
    }

    m_op->setData(result);

    QDialog::accept();
}

void EditDialog::reject()
{
    QDialog::reject();
}
