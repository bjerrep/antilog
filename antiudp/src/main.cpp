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

    QCommandLineOption forwardInputOption("f", "also forward input to stdout");
    parser.addOption(forwardInputOption);
    QCommandLineOption portOption("p", "port (default 12345)", "port", "12345");
    parser.addOption(portOption);
    parser.process(app);

    bool forwardToStdout = parser.isSet(forwardInputOption);
    int port = parser.value(portOption).toInt();

    QUdpSocket socket;
    socket.bind(QHostAddress::LocalHost, port);

    std::string lineInput;
    while (getline(std::cin,lineInput))
    {
        socket.writeDatagram(lineInput.c_str(), QHostAddress::LocalHost, port);
        if (forwardToStdout)
        {
            std::cout << lineInput << std::endl;
        }
    }

    exit(1);

    while( true) {
        std::string buffer;
        if ( ! (std::cin >> buffer) || buffer == "Disconnect")
            break;
        std::cout << buffer << "\n";
    }
    exit(1);
    while(!std::cin.eof())
    {
        char arr[1024];
        std::cin.read(arr, sizeof(arr));
        int length = std::cin.gcount();
        QByteArray data(arr, length);
        socket.writeDatagram(data, QHostAddress::LocalHost, port);
        if (forwardToStdout)
        {
            std::cout << arr;
        }
    }
}
