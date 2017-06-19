#pragma once

#include "inputbases.h"
#include "logentry.h"

#include <QObject>
#include <QJsonObject>

class SourceAndProcessor : public QObject
{
    Q_OBJECT

public:
    SourceAndProcessor(InputItemBase* source, InputItemBase* processor);
    SourceAndProcessor(const QJsonObject& json);
    ~SourceAndProcessor();

    QJsonObject getJson() const;
    bool isOn();
    void toggleEnable();
    void connectSignalsAndSlots();
    SourceBase* getSourceEntry() { return m_sourceEntry; }
    ProcessorBase* getProcessorEntry() { return m_processorEntry; }
    ProcessorBase* getProcessorEntry() const { return m_processorEntry; }

private:
    QString cleanMessage(QString message);

private slots:
    void slotSourceData(InputItemBase* source, QString message);
    void slotProcessorData(InputItemBase* source, LogEntryPtr logEntry);
public slots:
    void slotSystemReady();

signals:
    void signalNewData(InputItemBase* source, LogEntryPtr logEntry);
    void signalDataToProcessor(InputItemBase* source, QString message);
    void signalSystemReady();

private:
    SourceBase* m_sourceEntry = nullptr;
    ProcessorBase* m_processorEntry = nullptr;
};


