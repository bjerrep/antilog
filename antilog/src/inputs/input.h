#pragma once

#include "inputbases.h"
#include "logentry/logentry.h"

#include <QObject>
#include <QJsonObject>

class Input : public QObject
{
    Q_OBJECT

public:
    Input(InputItemBase* source, InputItemBase* processor);
    Input(const QJsonObject& json);
    ~Input();

    QJsonObject getJson() const;
    bool isOn();
    void enable(bool enable);
    void connectSignalsAndSlots();
    SourceBase* getSourceEntry() { return m_sourceEntry; }
    ProcessorBase* getProcessorEntry() { return m_processorEntry; }
    ProcessorBase* getProcessorEntry() const { return m_processorEntry; }

private:
    QString cleanMessage(QString message);

private slots:
    void slotSourceData(SourceBase* source, QString message, QString sourceIdentifier);
    void slotProcessorData(ProcessorBase* processor, LogEntryPtr logEntry);

public slots:
    void slotSystemReady();

signals:
    void signalNewData(ProcessorBase* processor, LogEntryPtr logEntry);
    void signalDataToProcessor(SourceBase* source, QString message, QString sourceIdentifier);
    void signalSystemReady();

private:
    SourceBase* m_sourceEntry = nullptr;
    ProcessorBase* m_processorEntry = nullptr;
};


