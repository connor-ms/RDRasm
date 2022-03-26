#include <QApplication>

#include "widgets/launchscreen.h"

#include <QFile>
#include <QFontDatabase>
#include <QMessageBox>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile stylesheet(":/res/light.qss");

    if (!stylesheet.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, "Warning", "Warning: unable to load stylesheet.");
    }

    a.setStyleSheet(QLatin1String(stylesheet.readAll()));

    QFontDatabase::addApplicationFont(":/res/fonts/RobotoMono-Regular.ttf");
    QFontDatabase::addApplicationFont(":/res/fonts/RobotoMono-Bold.ttf");

    QApplication::setWindowIcon(QIcon(":/res/icon.ico"));

    LaunchScreen ls;
    ls.show();

    return a.exec();
}
