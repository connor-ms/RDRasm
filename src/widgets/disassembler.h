#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>

#include <memory>

#include "../rage/iopcode.h"
#include "../rage/script.h"

namespace Ui {
class Disassembler;
}

class Disassembler : public QMainWindow
{
    Q_OBJECT

public:
    explicit Disassembler(QString file, QWidget *parent = nullptr);
    ~Disassembler();

private:
    void fillFuncTable(std::vector<std::shared_ptr<IOpcode>> funcs);

    QTextEdit    *createDisassemblyTab();
    QTableWidget *createStringsTab();

    void fillDisassembly(QTextEdit *disasm);

    QMap<unsigned int, QString> m_nativeMap;

    Ui::Disassembler *m_ui;
    Script m_script;
};

#endif // DISASSEMBLER_H
