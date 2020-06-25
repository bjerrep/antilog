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
    QCommandLineOption hostOption("host", "address to bind to (default localhost)", "host", "localhost");
    parser.addOption(portOption);
    parser.addOption(hostOption);
    parser.process(app);

    bool forwardToStdout = parser.isSet(forwardInputOption);
    int port = parser.value(portOption).toInt();

    QUdpSocket socket;
    QHostAddress host_address = QHostAddress::LocalHost;
    if (parser.value(hostOption).toUtf8() != "localhost")
    {
        host_address = QHostAddress(parser.value(hostOption));
    }

    std::string lineInput;
    while (getline(std::cin, lineInput))
    {
        socket.writeDatagram(lineInput.c_str(), host_address, port);
        if (forwardToStdout)
        {
            // stdout is buffered but how to disable that from within the application
            // remains to be figured out
            std::cout << lineInput << std::endl;
        }
    }
    socket.close();
}
