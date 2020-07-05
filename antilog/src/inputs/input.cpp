#include "input.h"
#include "sourcevisitors.h"
#include "inputsources.h"

#include <QThread>

Input::Input(InputItemBase* source, InputItemBase* processor)
{
    m_sourceEntry = static_cast<SourceBase*>(source);
    m_processorEntry = static_cast<ProcessorBase*>(processor);
    connectSignalsAndSlots();
}

Input::Input(const QJsonObject& json)
{
    auto source = json.value("source").toObject();
    m_sourceEntry = static_cast<SourceBase*>(InputEntryFactory(source));
    auto processor = json.value("processor").toObject();
    m_processorEntry = static_cast<ProcessorBase*>(InputEntryFactory(processor));
    connectSignalsAndSlots();
}

Input::~Input()
{
    delete m_sourceEntry;
    delete m_processorEntry;
}

QJsonObject Input::getJson() const
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

bool Input::isOn()
{
    return m_sourceEntry->getEnabled();
}

void Input::enable(bool enable)
{
    m_sourceEntry->setEnabled(enable);
}

void Input::connectSignalsAndSlots()
{
    connect(m_sourceEntry, &SourceBase::signalNewSourceData,
            this, &Input::slotSourceData,
            Qt::UniqueConnection);

    connect(this, &Input::signalDataToProcessor,
            m_processorEntry, &ProcessorBase::slotNewData,
            Qt::UniqueConnection);

    connect(this, &Input::signalSystemReady,
            m_sourceEntry, &SourceBase::slotSystemReady,
            Qt::QueuedConnection);

    connect(m_processorEntry, &ProcessorBase::signalNewProcessorData,
            this, &Input::slotProcessorData,
            Qt::UniqueConnection);
}

void Input::slotProcessorData(ProcessorBase* processor, LogEntryPtr logEntry)
{
    emit signalNewData(processor, logEntry);
}

void Input::slotSystemReady()
{
    emit signalSystemReady();
}

QString Input::cleanMessage(QString message)
{
    message = message.replace("\n", " ");
    return message;
}

void Input::slotSourceData(SourceBase* source, QString message, QString sourceIdentifier)
{
    QString cleaned = cleanMessage(message);
    emit signalDataToProcessor(source, cleaned, sourceIdentifier);
}
