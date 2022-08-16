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
    explicit Disassembler(QString file, bool debug, QWidget *parent = nullptr);
    ~Disassembler();

public slots:
    void exportDisassembly();
    void exportRawData();
    void exit();
    void open();

    void compilePS3();
    void compileX360();

private:
    void fillDisassembly();

    void createScriptDataTab();
    QTableWidget *createStringsTab();

    void createNativeTab();

    QString getResourceHeaderData();
    QString getScriptHeaderData();

    void compile(ScriptType type);

    QMap<unsigned int, QString> m_nativeMap;

    Ui::Disassembler *m_ui;
    Script m_script;
    QString m_file;
    OpcodeTable *m_disasm;
    bool m_debug;
};

#endif // DISASSEMBLER_H
