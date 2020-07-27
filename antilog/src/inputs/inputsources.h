#pragma once

#include "inputbases.h"
#include "inputsourcesudp.h"

#include <QString>
#include <QList>
#include <QJsonObject>

class FileReader;


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
    void relaunchFileReaderProcess();

private slots:
    void slotNewFileReaderData(const QString& data, const QString& sourceIdentifier);

protected:
    // FileSource attributes, used by configureFileReaderProcess().
    QString m_filepath;
    bool m_tailOnly = true;

protected:
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

    void save(QJsonObject& json) const override;
    void accept(InputVisitorBase* v) override;

private:
    void setFilenameAndConfigure(QString filename);

    friend class FileSourceDialog;
};

// ------ Statics -------

InputItemBase* InputEntryFactory(const QJsonObject& json);

void getAllSourceTypes(QList<InputItemBase*>& list);
