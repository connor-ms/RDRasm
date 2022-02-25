#include <QApplication>

//#include "rage/opcodebase.h"
#include "widgets/launchscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Global::opcodes = new OpcodesFromJson();

    LaunchScreen ls;
    ls.show();

    return a.exec();
}
