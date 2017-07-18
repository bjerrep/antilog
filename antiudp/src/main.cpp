#include <iostream>

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QByteArray>
#include <QUdpSocket>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("antiudp");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("antiudp, send input via udp to a listening antilog application.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption forwardInputOption("f", "also forward input to stdout "
                                               "(prefix with 'stdbuf -oL -eL' to disable console buffering)");
    parser.addOption(forwardInputOption);
    QCommandLineOption portOption("p", "port (default 12345)", "port", "12345");
    parser.addOption(portOption);
    parser.process(app);

    bool forwardToStdout = parser.isSet(forwardInputOption);
    int port = parser.value(portOption).toInt();

    QUdpSocket socket;
    socket.bind(QHostAddress::LocalHost, port);

    std::string lineInput;
    while (getline(std::cin, lineInput))
    {
        socket.writeDatagram(lineInput.c_str(), QHostAddress::LocalHost, port);
        if (forwardToStdout)
        {
            // stdout is buffered but how to disable that from within the application
            // remains to be figured out
            std::cout << lineInput << std::endl;
        }
    }
}
