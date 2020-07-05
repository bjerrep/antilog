#include "inputlist.h"
#include "antilog.h"
#include "input.h"
#include "format/formatscheme.h"

#include <QJsonArray>

InputList::InputList(AntiLog* antiLog)
    : m_app(antiLog)
{
}

InputList::~InputList()
{
    qDeleteAll(m_inputList);
}

void InputList::save(QJsonObject& json) const
{
    QJsonArray inputs;
    foreach (auto input, m_inputList)
    {
        auto json = input->getJson();
        inputs.append(json);
    }
    json[Statics::Inputs] = inputs;
}

void InputList::load(const QJsonObject& json)
{
    auto inputs = json[Statics::Inputs].toArray();

    foreach (auto jsoninput, inputs)
    {
        auto jsonObject = jsoninput.toObject();
        auto input = new Input(jsonObject);
        appendAndConnect(input);
    }
    Statics::SystemReady = true;
    foreach (auto input, m_inputList)
    {
        input->slotSystemReady();
    }
}

int InputList::count() const
{
    return m_inputList.count();
}

void InputList::appendAndConnect(Input* input)
{
    m_inputList.append(input);
    connect(input->getSourceEntry(), &SourceBase::signalNewSourceData,
            m_app, &AntiLog::slotSourceTrafficMonitor);

    connect(input->getProcessorEntry(), &ProcessorBase::signalNewProcessorData,
            m_app, &AntiLog::slotNewLogEntry);
}

void InputList::removeAt(int index)
{
    auto removed = m_inputList.takeAt(index);
    delete removed;
}

int InputList::findFormatSchemeUsageCount(const QString& schemeName) const
{
    int count = 0;
    foreach (auto input, m_inputList)
    {
        if (input->getProcessorEntry()->getScheme()->getName() == schemeName)
        {
            ++count;
        }
    }
    return count;
}

Input* InputList::getInput(int index)
{
    return m_inputList[index];
}

Input* InputList::getInput(int index) const
{
    return m_inputList[index];
}

InputListType InputList::getAllInputs()
{
    return m_inputList;
}

InputItemVector InputList::getAllItemInputs() const
{
    InputItemVector list;
    foreach (auto input, m_inputList)
    {
        list.append(input->getSourceEntry());
        list.append(input->getProcessorEntry());
    }
    return list;
}
