#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <QMainWindow>
#include <QTextEdit>

#include <memory>

#include "opcodetable.h"
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

public slots:
    void exportDisassembly();
    void exportRawData();

private:
    void fillFuncTable(std::vector<std::shared_ptr<IOpcode>> funcs);
    void fillDisassembly();

    void createDisassemblyTab();
    void createScriptDataTab();
    QTableWidget *createStringsTab();

    QString getResourceHeaderData();
    QString getScriptHeaderData();

    QMap<unsigned int, QString> m_nativeMap;

    Ui::Disassembler *m_ui;
    Script m_script;
    QString m_file;
    OpcodeTable *m_disasm;
};

#endif // DISASSEMBLER_H
