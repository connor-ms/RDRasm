#include "launchscreen.h"
#include "ui_launchscreen.h"

#include <QDesktopServices>
#include <QFileDialog>

#include "disassembler.h"

LaunchScreen::LaunchScreen(QWidget *parent) :
    QWidget(parent),
    m_Ui(new Ui::LaunchScreen)
{
    m_Ui->setupUi(this);

    connect(m_Ui->btnOpenFile, SIGNAL(clicked()), this, SLOT(openFilePressed()));
    connect(m_Ui->btnGithub, SIGNAL(clicked()), this, SLOT(githubPressed()));
    connect(m_Ui->btnExit, SIGNAL(clicked()), this, SLOT(exitPressed()));
}

LaunchScreen::~LaunchScreen()
{
    delete m_Ui;
}

void LaunchScreen::openFilePressed()
{
    QString file = QFileDialog::getOpenFileName(this, "Select a file", "", "Script (*.xsc)");

    if (!file.isEmpty())
    {
        Disassembler *dsm = new Disassembler(file);
        dsm->show();

        close();
    }
}

void LaunchScreen::githubPressed()
{
    QDesktopServices::openUrl(QUrl("www.github.com/connorms/ScriptDisassembler"));
}

void LaunchScreen::exitPressed()
{
    QApplication::exit();
}
