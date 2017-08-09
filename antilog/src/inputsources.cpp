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
#include <QDir>
#include <QFileInfoList>


// ------ DirSource -------

DirSource::DirSource(const QJsonObject& json)
    : SourceBase(__func__, json)
{
    if (json.empty())
    {
        setName(makeNameUnique("Dir"));
        m_dir = "/var/log";
        m_mask = "*";
        return;
    }

    m_dir = json.value(Statics::FilePath).toString();
    m_recursive = json.value("recursive").toBool();
    m_mask = json.value("mask").toString();
    if (m_dir.isEmpty())
        setDescription("not configured");
    else
        setDescription(m_dir);
    relaunchFileReaderProcess();
}

DirSource::DirSource(const QString& type, const QJsonObject& json)
    : SourceBase(type, json)
{
}

DirSource::~DirSource()
{
    delete m_fileReader;
}

void DirSource::save(QJsonObject& json) const
{
    SourceBase::save(json);
    json[Statics::FilePath] = m_dir;
    json["recursive"] = m_recursive;
    json["mask"] = m_mask;
}

void DirSource::slotNewFileReaderData(const QString& data, const QString& sourceIdentifier)
{
    emit signalNewSourceData(this, data, getName() + " " + sourceIdentifier);
}

void DirSource::slotSystemReady()
{
    SourceBase::slotSystemReady();
    relaunchFileReaderProcess();
}

void DirSource::accept(InputVisitorBase* v)
{
    v->visit(this);
}

void DirSource::relaunchFileReaderProcess()
{
    if (isGoodToGo())
    {
        delete m_fileReader;

        if (!m_dir.isEmpty())
        {
            m_fileReader = new FileReader(m_dir, true, "*");
        }
        else
        {
            const int linesToLoad = Statics::s_options->m_numberOfLinesToLoad * !m_tailOnly;
            m_fileReader = new FileReader(m_filepath, linesToLoad);
        }

        connect(m_fileReader, &FileReader::signalNewData,
                this, &DirSource::slotNewFileReaderData,
                Qt::QueuedConnection);
        if (Statics::SystemReady)
        {
            m_fileReader->startReader();
        }

    }
    else
    {
        delete m_fileReader;
        m_fileReader = nullptr;
    }
}

void DirSource::setDir(QString dir)
{
    setDescription(dir);
    m_dir = dir;
    relaunchFileReaderProcess();
}

void DirSource::setEnabled(bool enabled)
{
    SourceBase::setEnabled(enabled);
    relaunchFileReaderProcess();
}

// ------ FileSource -------

FileSource::FileSource(const QJsonObject& json)
    : DirSource(__func__, json)
{
    if (json.empty())
    {
        setName(makeNameUnique("File"));
        return;
    }

    m_filepath = json.value(Statics::FilePath).toString();
    m_tailOnly = json[Statics::TailOnly].toBool();
    if (m_filepath.isEmpty())
        setDescription("not configured");
    else
        setDescription(m_filepath);
    relaunchFileReaderProcess();
}

void FileSource::save(QJsonObject& json) const
{
    SourceBase::save(json);
    json[Statics::FilePath] = m_filepath;
    json[Statics::TailOnly] = m_tailOnly;
}

void FileSource::accept(InputVisitorBase* v)
{
    v->visit(this);
}

void FileSource::setFilenameAndConfigure(QString filename)
{
    setDescription(filename);
    m_filepath = filename;
    relaunchFileReaderProcess();
}

// ------ UDPSource -------

UDPSource::UDPSource(const QJsonObject& json)
    : SourceBase(__func__, json)
{
    if (json.empty())
    {
        setName(makeNameUnique("UDP"));
        setPort(m_port);
        return;
    }

    setPort(json["port"].toString().toInt());
}

UDPSource::~UDPSource()
{
    if (m_socket)
    {
        m_socket->reset();
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
            warn("error binding udpsocket " << m_port);
        }
        if (m_socket->state() == QUdpSocket::BoundState)
        {
            connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotNewUdpSocketData()));
        }
    }
    m_sourceDescriptor = getName() + " " + QString::number(m_port);
}

void UDPSource::slotSystemReady()
{
    SourceBase::slotSystemReady();
    if (isGoodToGo())
    {
        setEnabled(true);
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

        emit signalNewSourceData(this, QString::fromLatin1(buffer), m_sourceDescriptor);
    }
}

void UDPSource::setEnabled(bool enabled)
{
    SourceBase::setEnabled(enabled);
    if (!m_socket && enabled)
    {
        openSocket();
        if (m_socket->state() != QUdpSocket::BoundState)
        {
            emit signalNewSourceData(this,
                                     Statics::AntiLogMessage + getName() + " failed to open socket",
                                     m_sourceDescriptor);
        }
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
    if (isGoodToGo())
    {
        setEnabled(false);
        setEnabled(true);
    }
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
    else if (className == DirSource().getClassName())
    {
        return new DirSource(json);
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
    list.append(new DirSource());
    list.append(new UDPSource());
}

