#include "antilog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AntiLog antilog;
    //experimental api
    //antilog.reconfigureUdpSources("any", 12345);
    antilog.show();

    return a.exec();
}
