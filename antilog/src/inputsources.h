#pragma once

#include "inputbases.h"

#include <QString>
#include <QList>
#include <QJsonObject>

class InputVisitorBase;
class FileSource;
class UDPSource;
class PassProcessor;
class RegexProcessor;
class FileReader;
class QUdpSocket;

class FileSource : public SourceBase
{
    Q_OBJECT

public:
    FileSource(const QJsonObject& json = QJsonObject());
    ~FileSource();

    void save(QJsonObject& json) const override;
    void accept(InputVisitorBase* v);
    void setFilename(QString filename);
    QString getFilename() const;
    void setEnabled(bool enabled) override;

public slots:
    void slotSystemReady() override;

private:
    void configureFileReaderProcess();

private slots:
    void slotNewFileReaderData(QString data);

private:
    FileReader* m_fileReader = nullptr;
    bool m_tailOnly = true;
    QString m_filePath;

    friend class FileSourceDialog;
};


class UDPSource : public SourceBase
{
    Q_OBJECT

public:
    UDPSource(const QJsonObject& json = QJsonObject());
    ~UDPSource();

    void save(QJsonObject& json) const override;
    void accept(InputVisitorBase* v);
    void setEnabled(bool enabled);
    int getPort() const;
    void setPort(int port);

private:
    void openSocket();

private slots:
    void slotNewUdpSocketData();

private:
    QUdpSocket* m_socket = nullptr;
    int m_port = 12345;
};

// ------ Statics -------

InputItemBase* InputEntryFactory(const QJsonObject& json);

void getAllSources(QList<InputItemBase*>& list);
