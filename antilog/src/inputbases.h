#pragma once

#include "logentry.h"

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

private:
    QString m_name = "Default";
    QString m_description = "No description";
    QString m_className = "Unknown";
};


using InputItemList = QList<InputItemBase*>;

class SourceBase : public InputItemBase
{
    Q_OBJECT

public:
    SourceBase(const QString& getClassName, const QJsonObject& json);
    virtual ~SourceBase();

    virtual void setEnabled(bool enabled);
    bool enabled() const;
    virtual void save(QJsonObject& json) const override = 0;

public slots:
    virtual void slotSystemReady();

signals:
    void signalNewSourceData(InputItemBase* source, QString data);

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
    InputItemBase* getInputItem();
    void setInputItem(InputItemBase* inputItem);
    virtual void save(QJsonObject& json) const override = 0;

    const char* SchemeNameKey = "schemename";

signals:
    void signalNewProcessorData(InputItemBase* source, LogEntryPtr logEntry);

public slots:
    virtual void slotNewData(InputItemBase* source, QString data) = 0;

private:
    InputItemBase* m_inputItem = nullptr;
    FormatScheme* m_formatScheme = nullptr;
};
