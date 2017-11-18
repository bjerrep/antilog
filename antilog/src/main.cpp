#include "antilog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AntiLog antilog;
    antilog.show();

    return a.exec();
}
