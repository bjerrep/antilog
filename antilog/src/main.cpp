#include "antilog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AntiLog ulw;
    ulw.show();

    return a.exec();
}
