#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

#include "../rage/iopcode.h"

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(std::shared_ptr<IOpcode> op, QWidget *parent = nullptr);
    ~EditDialog();

private slots:
    void accept();
    void reject();

private:
    Ui::EditDialog *m_ui;

    std::shared_ptr<IOpcode> m_op;
};

#endif // EDITDIALOG_H
