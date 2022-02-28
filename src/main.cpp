#include <QApplication>

#include "widgets/launchscreen.h"

#include <QFile>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile stylesheet(":/res/light.qss");

    stylesheet.open(QIODevice::ReadOnly);

    QString test = QLatin1String(stylesheet.readAll());

    a.setStyleSheet(test);

    LaunchScreen ls;
    ls.show();

    return a.exec();
}
