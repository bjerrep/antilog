#include "sourceandprocessor.h"
#include "sourcevisitors.h"
#include "inputsources.h"
#include "logentry.h"

#include <QThread>

SourceAndProcessor::SourceAndProcessor(InputItemBase* source, InputItemBase* processor)
{
    m_sourceEntry = static_cast<SourceBase*>(source);
    m_processorEntry = static_cast<ProcessorBase*>(processor);
    connectSignalsAndSlots();
}

SourceAndProcessor::SourceAndProcessor(const QJsonObject& json)
{
    auto source = json.value("source").toObject();
    m_sourceEntry = static_cast<SourceBase*>(InputEntryFactory(source));
    auto processor = json.value("processor").toObject();
    m_processorEntry = static_cast<ProcessorBase*>(InputEntryFactory(processor));
    connectSignalsAndSlots();
}

SourceAndProcessor::~SourceAndProcessor()
{
    delete m_sourceEntry;
    delete m_processorEntry;
}

QJsonObject SourceAndProcessor::getJson() const
{
    GetJson getJsonSource;
    m_sourceEntry->accept(&getJsonSource);
    GetJson getJsonProcessor;
    m_processorEntry->accept(&getJsonProcessor);
    QJsonObject input;
    input[Statics::Source] = getJsonSource.m_json;
    input[Statics::Processor] = getJsonProcessor.m_json;

    return input;
}

bool SourceAndProcessor::isOn()
{
    return m_sourceEntry->enabled();
}

void SourceAndProcessor::toggleEnable()
{
    m_sourceEntry->setEnabled(!m_sourceEntry->enabled());
}

void SourceAndProcessor::connectSignalsAndSlots()
{
    connect(m_sourceEntry, &SourceBase::signalNewSourceData,
            this, &SourceAndProcessor::slotSourceData,
            Qt::UniqueConnection);

    connect(this, &SourceAndProcessor::signalDataToProcessor,
            m_processorEntry, &ProcessorBase::slotNewData,
            Qt::UniqueConnection);

    connect(this, &SourceAndProcessor::signalSystemReady,
            m_sourceEntry, &SourceBase::slotSystemReady,
            Qt::QueuedConnection);

    connect(m_processorEntry, &ProcessorBase::signalNewProcessorData,
            this, &SourceAndProcessor::slotProcessorData,
            Qt::UniqueConnection);
}

void SourceAndProcessor::slotProcessorData(ProcessorBase* processor, LogEntryPtr logEntry)
{
    emit signalNewData(processor, logEntry);
}

void SourceAndProcessor::slotSystemReady()
{
    emit signalSystemReady();
}

QString SourceAndProcessor::cleanMessage(QString message)
{
    message = message.replace("\n", " ");
    return message;
}

void SourceAndProcessor::slotSourceData(SourceBase* source, QString message, QString sourceIdentifier)
{
    QString cleaned = cleanMessage(message);
    emit signalDataToProcessor(source, cleaned, sourceIdentifier);
}
