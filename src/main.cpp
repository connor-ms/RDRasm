#include <QApplication>

#include "widgets/launchscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LaunchScreen ls;
    ls.show();

    return a.exec();
}
