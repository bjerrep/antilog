#include "inputlist.h"
#include "antilog.h"
#include "sourceandprocessor.h"
#include "formatscheme.h"
#include "statics.h"

#include <QJsonArray>

InputList::InputList(AntiLog* ulw)
    : m_app(ulw)
{
}

InputList::~InputList()
{
    qDeleteAll(m_sourceAndProcessorList);
}

void InputList::save(QJsonObject& json) const
{
    QJsonArray inputs;
    foreach (auto sourceAndProcessor, m_sourceAndProcessorList)
    {
        auto input = sourceAndProcessor->getJson();
        inputs.append(input);
    }
    json[Statics::Inputs] = inputs;
}

void InputList::load(const QJsonObject& json)
{
    auto inputs = json[Statics::Inputs].toArray();

    foreach (auto input, inputs)
    {
        auto jsonObject = input.toObject();
        auto sourceAndProcessor = new SourceAndProcessor(jsonObject);
        appendAndConnect(sourceAndProcessor);
    }
    Statics::SystemReady = true;
    foreach (auto sourceAndProcessor, m_sourceAndProcessorList)
    {
        sourceAndProcessor->slotSystemReady();
    }
}

int InputList::count() const
{
    return m_sourceAndProcessorList.count();
}

void InputList::appendAndConnect(SourceAndProcessor* input)
{
    m_sourceAndProcessorList.append(input);
    connect(input->getProcessorEntry(), &ProcessorBase::signalNewProcessorData,
            m_app, &AntiLog::slotNewLogEntry);
}

void InputList::removeAt(int index)
{
    auto removed = m_sourceAndProcessorList.takeAt(index);
    delete removed;
}

int InputList::findFormatSchemeUsageCount(const QString& schemeName) const
{
    int count = 0;
    foreach (auto sourceAndProcessor, m_sourceAndProcessorList)
    {
        if (sourceAndProcessor->getProcessorEntry()->getScheme()->name() == schemeName)
        {
            ++count;
        }
    }
    return count;
}

SourceAndProcessor* InputList::getSourceAndProcessor(int index)
{
    return m_sourceAndProcessorList[index];
}

SourceAndProcessor* InputList::getSourceAndProcessor(int index) const
{
    return m_sourceAndProcessorList[index];
}
