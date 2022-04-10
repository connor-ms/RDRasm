#include "launchscreen.h"
#include "ui_launchscreen.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>

#include "disassembler.h"

#include "../rage/opcodefactory.h"
#include "../util/util.h"

LaunchScreen::LaunchScreen(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::LaunchScreen)
{
    m_ui->setupUi(this);

    if (Util::getAESKey() == QByteArray())
    {
        m_ui->btnOpenFile->setEnabled(false);
    }

    connect(m_ui->btnOpenFile, SIGNAL(clicked()), this, SLOT(openFilePressed()));
    connect(m_ui->btnGithub,   SIGNAL(clicked()), this, SLOT(githubPressed()));
    connect(m_ui->btnExit,     SIGNAL(clicked()), this, SLOT(exitPressed()));
}

LaunchScreen::~LaunchScreen()
{
    delete m_ui;
}

void LaunchScreen::openFilePressed()
{
    QString file = QFileDialog::getOpenFileName(this, "Select a file", "", "Script (*.xsc *.csc)");

    if (!file.isEmpty())
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        Disassembler *dsm = new Disassembler(file);
        dsm->show();

        close();
    }
}

void LaunchScreen::githubPressed()
{
    QDesktopServices::openUrl(QUrl("www.github.com/connorms/RDRasm"));
}

void LaunchScreen::exitPressed()
{
    QApplication::exit();
}
