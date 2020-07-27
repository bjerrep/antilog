#include "inputsourcesudp.h"
#include "inputvisitorbase.h"

#include <QUdpSocket>
#include <QJsonObject>

// ------ UDPSourceBase -------

UDPSourceBase::UDPSourceBase(const QString& name, const QJsonObject& json)
    : SourceBase(name, json)
{
    if (json.empty())
    {
        setName(makeNameUnique(name));
        setAddress(m_address);
        setPort(m_port);
        return;
    }

    setAddress(json["address"].toString());
    setPort(json["port"].toString().toInt());
}

UDPSourceBase::~UDPSourceBase()
{
    if (m_socket)
    {
        m_socket->reset();
        m_socket->close();
        delete m_socket;
    }
}

void UDPSourceBase::save(QJsonObject& json) const
{
    SourceBase::save(json);
    json["address"] = m_address;
    json["port"] = QString::number(m_port);
}

void UDPSourceBase::slotSystemReady()
{
    start();
}

void UDPSourceBase::setEnabled(bool enabled)
{
    SourceBase::setEnabled(enabled);
    start();
}

void UDPSourceBase::slotNewUdpSocketData()
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

void UDPSourceBase::start()
{
    if (!m_socket && isGoodToGo())
    {
        openSocket();
        if (m_socket->state() != QUdpSocket::BoundState)
        {
            emit signalNewSourceData(this,
                                     Statics::AntiLogMessage + getName() + " failed to open socket",
                                     m_sourceDescriptor);
        }
    }
    else if (m_socket && !isGoodToGo())
    {
        m_socket->close();
        delete m_socket;
        m_socket = nullptr;
    }
}

QString UDPSourceBase::getAddress() const
{
    return m_address;
}

void UDPSourceBase::setAddress(QString address)
{
    m_address = address;
    if (isGoodToGo())
    {
        setEnabled(false);
        setEnabled(true);
    }
    setDescription(m_address + ":" + QString::number(m_port));
}

int UDPSourceBase::getPort() const
{
    return m_port;
}

void UDPSourceBase::setPort(int port)
{
    m_port = port;
    if (isGoodToGo())
    {
        setEnabled(false);
        setEnabled(true);
    }
    setDescription(m_address + ":" + QString::number(m_port));
}

// ------ UDPSource -------

void UDPSource::openSocket()
{
    if (!m_socket)
    {
        m_socket = new QUdpSocket(this);
    }
    if (m_socket->state() != QUdpSocket::BoundState)
    {
        if (!m_socket->bind(QHostAddress(m_address), m_port))
        {
            warn("error binding udpsocket " << m_port);
        }
        if (m_socket->state() == QUdpSocket::BoundState)
        {
            connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotNewUdpSocketData()));
        }
    }
    m_sourceDescriptor = getName() + " " + m_address + ":" + QString::number(m_port);
}

void UDPSource::accept(InputVisitorBase* v)
{
    v->visit(this);
}

// ------ UDPMulticastSource -------

void UDPMulticastSource::openSocket()
{
    if (!m_socket)
    {
        m_socket = new QUdpSocket(this);
    }
    if (m_socket->state() != QUdpSocket::BoundState)
    {
        if (!m_socket->bind(QHostAddress::AnyIPv4, m_port, QUdpSocket::ShareAddress))
        {
            warn("error binding udpsocket " << m_port);
        }
        if (!m_socket->joinMulticastGroup(QHostAddress(m_address)))
        {
            warn("error joining multicast group " << m_address << ":" << m_port);
        }
        if (m_socket->state() == QUdpSocket::BoundState)
        {
            connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotNewUdpSocketData()));
        }
    }
    m_sourceDescriptor = getName() + " " + m_address + ":" + QString::number(m_port);
}

void UDPMulticastSource::accept(InputVisitorBase* v)
{
    v->visit(this);
}

