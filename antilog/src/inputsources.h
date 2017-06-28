#pragma once

#include "inputbases.h"

#include <QString>
#include <QList>
#include <QJsonObject>

class FileReader;
class QUdpSocket;


class DirSource : public SourceBase
{
    Q_OBJECT

public:
    DirSource(const QJsonObject& json = QJsonObject());
    DirSource(const QString& type, const QJsonObject& json);
    virtual ~DirSource();

    void save(QJsonObject& json) const override;
    void accept(InputVisitorBase* v) override;
    void setDir(QString dir);
    void setEnabled(bool enabled) override;

public slots:
    void slotSystemReady() override;

protected:
    void configureFileReaderProcess();

private slots:
    void slotNewFileReaderData(QString data, QString sourceIdentifier);

protected:
    // FileSource attributes, used by configureFileReaderProcess().
    QString m_filepath;
    bool m_tailOnly = true;

private:
    FileReader* m_fileReader = nullptr;
    QString m_dir;
    bool m_recursive = true;
    QString m_mask;
    friend class DirSourceDialog;
};


class FileSource : public DirSource
{
    Q_OBJECT

public:
    FileSource(const QJsonObject& json = QJsonObject());
    //~FileSource();

    void save(QJsonObject& json) const override;
    void accept(InputVisitorBase* v) override;
    void setFilenameAndConfigure(QString filename);

    friend class FileSourceDialog;
};

class UDPSource : public SourceBase
{
    Q_OBJECT

public:
    UDPSource(const QJsonObject& json = QJsonObject());
    ~UDPSource();

    void save(QJsonObject& json) const override;
    void accept(InputVisitorBase* v) override;
    void setEnabled(bool enabled) override;
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
