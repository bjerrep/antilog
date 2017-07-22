#include "inputbases.h"
#include "formatschememodel.h"
#include "statics.h"

#include <QJsonObject>

InputItemBase::InputItemBase(const QString& type, const QJsonObject& json)
    : m_className(type)
{
    if (json.empty())
        return;
    m_name = json[Statics::Name].toString();
    m_description = json[Statics::Description].toString();
}

InputItemBase::~InputItemBase()
{
}

const QString& InputItemBase::getName() const
{
    return m_name;
}

void InputItemBase::setName(const QString& name)
{
    m_name = name;
}

QString InputItemBase::getDescription() const
{
    return m_description;
}

void InputItemBase::setDescription(const QString& description)
{
    m_description = description;
}

QString InputItemBase::getClassName()
{
    return m_className;
}

void InputItemBase::save(QJsonObject& json) const
{
    json[Statics::ClassName] = m_className;
    json[Statics::Name] = m_name;
    json[Statics::Description] = m_description;
}

// ------ SourceItem -------

SourceBase::SourceBase(const QString& type, const QJsonObject& json)
    : InputItemBase(type, json)
{
    if (json.empty())
        return;

    m_enabled = json.value(Statics::Enabled).toBool();
}

SourceBase::~SourceBase()
{
}

void SourceBase::save(QJsonObject& json) const
{
    json[Statics::Enabled] = m_enabled;
    InputItemBase::save(json);
}

bool SourceBase::enabled() const
{
    return m_enabled;
}

bool SourceBase::isGoodToGo() const
{
    return m_systemReady && m_enabled;
}

void SourceBase::setEnabled(bool enabled)
{
    m_systemReady = true;
    m_enabled = enabled;
}

void SourceBase::slotSystemReady()
{
    m_systemReady = true;
}

// ------ ProcessorItem -------

ProcessorBase::ProcessorBase(QString type, const QJsonObject& json)
    : InputItemBase(type, json)
{
}

ProcessorBase::~ProcessorBase()
{
}

void ProcessorBase::setSchemeFromName(const QString& scheme)
{
    m_formatScheme = Statics::formatSchemeModel->getFormatScheme(scheme);
}

FormatScheme* ProcessorBase::getScheme() const
{
    return m_formatScheme;
}

InputItemBase* ProcessorBase::getInputItem()
{
    return m_inputItem;
}

void ProcessorBase::setInputItem(InputItemBase* inputItem)
{
    m_inputItem = inputItem;
}

void ProcessorBase::save(QJsonObject& json) const
{
    InputItemBase::save(json);
}
