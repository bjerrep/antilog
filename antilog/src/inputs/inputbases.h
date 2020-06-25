#pragma once

#include "logentry/logentry.h"

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QList>

class InputVisitorBase;
class FormatScheme;

class InputItemBase : public QObject
{
    Q_OBJECT

public:
    InputItemBase(const QString& getClassName, const QJsonObject& json);
    InputItemBase(const InputItemBase& org);
    virtual ~InputItemBase();

    virtual void accept(InputVisitorBase* v) = 0;
    QString getClassName();
    const QString& getName() const;
    void setName(const QString& getName);
    QString getDescription() const;
    void setDescription(const QString& getDescription);
    virtual void save(QJsonObject& json) const = 0;
    QString makeNameUnique(const QString& name) const;

private:
    QString m_name = "Default";
    QString m_description = "No description";
    QString m_className = "Unknown";
};


using InputItemList = QList<InputItemBase*>;
using InputItemVector = QVector<InputItemBase*>;

class SourceBase : public InputItemBase
{
    Q_OBJECT

public:
    SourceBase(const QString& getClassName, const QJsonObject& json);
    virtual ~SourceBase();

    virtual void save(QJsonObject& json) const override = 0;

    virtual void setEnabled(bool enabled);
    bool getEnabled() const;
    bool isGoodToGo() const;

public slots:
    virtual void slotSystemReady() = 0;

signals:
    void signalNewSourceData(SourceBase* source, QString data, QString sourceIdentifier);

private:
    bool m_enabled = false;
};


class ProcessorBase : public InputItemBase
{
    Q_OBJECT

public:
    ProcessorBase(QString getClassName, const QJsonObject& json);
    virtual ~ProcessorBase();

    virtual void setSchemeFromName(const QString& scheme);
    FormatScheme* getScheme() const;
    void setInputItem(InputItemBase* inputItem);
    virtual void save(QJsonObject& json) const override = 0;

    const char* SchemeNameKey = "schemename";

signals:
    void signalNewProcessorData(ProcessorBase* processor, LogEntryPtr logEntry);

public slots:
    virtual void slotNewData(SourceBase* source, QString data, QString sourceIdentifier) = 0;

private:
    InputItemBase* m_inputItemBase = nullptr;
    FormatScheme* m_formatScheme = nullptr;
};
