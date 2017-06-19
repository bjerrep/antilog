#include "inputsources.h"
#include "inputprocessors.h"
#include "filereader.h"
#include "inputvisitorbase.h"
#include "options.h"

#include <QList>
#include <QByteArray>
#include <QDebug>
#include <QUdpSocket>
#include <QJsonObject>

// ------ FileSource -------

FileSource::FileSource(const QJsonObject& json)
    : SourceBase(__func__, json)
{
    if (json.empty())
    {
        setName("File");
        return;
    }

    m_filePath = json.value(Statics::FilePath).toString();
    m_tailOnly = json[Statics::TailOnly].toBool();
    if (m_filePath.isEmpty())
        setDescription("not configured");
    else
        setDescription(m_filePath);
    configureFileReaderProcess();
}

FileSource::~FileSource()
{
    delete m_fileReader;
}

void FileSource::save(QJsonObject& json) const
{
    SourceBase::save(json);
    json[Statics::FilePath] = m_filePath;
    json[Statics::TailOnly] = m_tailOnly;
}

void FileSource::slotNewFileReaderData(QString data)
{
    emit signalNewSourceData(this, data);
}

void FileSource::slotSystemReady()
{
    QMetaObject::invokeMethod(m_fileReader, "systemReady", Qt::QueuedConnection);
}

void FileSource::accept(InputVisitorBase* v)
{
    v->visit(this);
}

void FileSource::configureFileReaderProcess()
{
    if (enabled())
    {
        delete m_fileReader;
        const int linesToLoad = Statics::options->m_numberOfLinesToLoad * !m_tailOnly;
        m_fileReader = new FileReader(m_filePath, linesToLoad);
        connect(m_fileReader, &FileReader::signalNewData,
                this, &FileSource::slotNewFileReaderData,
                Qt::QueuedConnection);
        if (Statics::SystemReady)
            m_fileReader->start();
    }
    else
    {
        delete m_fileReader;
        m_fileReader = nullptr;
    }
}

void FileSource::setFilename(QString filename)
{
    setDescription(filename);
    m_filePath = filename;
    configureFileReaderProcess();
}

QString FileSource::getFilename() const
{
    return m_filePath;
}

void FileSource::setEnabled(bool enabled)
{
    SourceBase::setEnabled(enabled);
    configureFileReaderProcess();
}

// ------ UDPSource -------

UDPSource::UDPSource(const QJsonObject& json)
    : SourceBase(__func__, json)
{
    if (json.empty())
    {
        setName("UDP");
        setPort(m_port);
        return;
    }

    if (enabled())
    {
        setEnabled(true);
    }

    setPort(json["port"].toString().toInt());
}

UDPSource::~UDPSource()
{
    if (m_socket)
    {
        m_socket->close();
        delete m_socket;
    }
}

void UDPSource::save(QJsonObject& json) const
{
    SourceBase::save(json);
    json["port"] = QString::number(m_port);
}

void UDPSource::accept(InputVisitorBase* v)
{
    v->visit(this);
}

void UDPSource::openSocket()
{
    m_socket = new QUdpSocket(this);
    if (m_socket->state() != QUdpSocket::BoundState)
    {
        if (!m_socket->bind(QHostAddress::LocalHost, m_port))
        {
            warn("error binding udpsocket " + m_port);
        }
        if (m_socket->state() == QUdpSocket::BoundState)
        {
            connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotNewUdpSocketData()));
        }
    }
}

void UDPSource::slotNewUdpSocketData()
{
    QByteArray buffer;

    while(m_socket->hasPendingDatagrams())
    {
        buffer.resize(m_socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        m_socket->readDatagram(buffer.data(),
                               buffer.size(),
                               &sender,
                               &senderPort);

        emit signalNewSourceData(this, QString::fromLatin1(buffer));
    }
}

void UDPSource::setEnabled(bool enabled)
{
    SourceBase::setEnabled(enabled);
    if (!m_socket && enabled)
    {
        openSocket();
    }
    else if (m_socket && !enabled)
    {
        m_socket->close();
        delete m_socket;
        m_socket = nullptr;
    }
}

int UDPSource::getPort() const
{
    return m_port;
}

void UDPSource::setPort(int port)
{
    m_port = port;
    setDescription("Port " + QString::number(m_port));
}

// ------ Statics -------

InputItemBase* InputEntryFactory(const QJsonObject& json)
{
    QString className = json[Statics::ClassName].toString();

    if (className == FileSource().getClassName())
    {
        return new FileSource(json);
    }
    else if (className == UDPSource().getClassName())
    {
        return new UDPSource(json);
    }
    else if (className == PassProcessor().getClassName())
    {
        return new PassProcessor(json);
    }
    else if (className == RegexProcessor().getClassName())
    {
        return new RegexProcessor(json);
    }

    warn(QString("InputEntryFactory missing type %1").arg(className));
    return nullptr;
}

void getAllSources(InputItemList& list)
{
    list.append(new FileSource());
    list.append(new UDPSource());
}

