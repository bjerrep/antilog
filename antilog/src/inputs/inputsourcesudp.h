#pragma once

#include "inputbases.h"

#include <QString>
#include <QJsonObject>

class QUdpSocket;

class UDPSourceBase : public SourceBase
{
    Q_OBJECT

public:
    UDPSourceBase(const QString& name, const QJsonObject& json);
    ~UDPSourceBase();

    void save(QJsonObject& json) const override;
    void setEnabled(bool enabled) override;
    void start();
    QString getAddress() const;
    void setAddress(QString address);
    int getPort() const;
    void setPort(int port);

private:
    virtual void openSocket() = 0;

private slots:
    void slotSystemReady() override;
    void slotNewUdpSocketData();

private:
    QUdpSocket* m_socket = nullptr;
    QString m_address = "localhost";
    int m_port = 12345;
    QString m_sourceDescriptor;

    friend class UDPSource;
    friend class UDPMulticastSource;
};


class UDPSource : public UDPSourceBase
{
public:
    UDPSource(const QJsonObject& json = QJsonObject()) : UDPSourceBase(__func__, json) {};
    void openSocket() override;
    void accept(InputVisitorBase* v);
};


class UDPMulticastSource : public UDPSourceBase
{
public:
    UDPMulticastSource(const QJsonObject& json = QJsonObject()) : UDPSourceBase(__func__, json) {};
    void openSocket() override;
    void accept(InputVisitorBase* v);
};

