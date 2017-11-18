#include "formatscheme.h"
#include "formatrule.h"


FormatScheme::FormatScheme(const QJsonObject& json)
    : m_tableFormat(json)
{
    if (json.empty())
    {
        return;
    }
    m_name = json["name"].toString();
    auto inputs = json["setup"].toArray();
    foreach (auto input, inputs)
    {
        auto scheme = input.toObject();
        auto formatEntry = new FormatRule(scheme);
        m_formatRuleList.append(formatEntry);
    }
}

FormatScheme::FormatScheme(const QString& name)
    : m_name(name)
{
}

FormatScheme::~FormatScheme()
{
    qDeleteAll(m_formatRuleList);
}

void FormatScheme::save(QJsonObject& json) const
{
    QJsonArray result;
    foreach (auto formatEntry, m_formatRuleList)
    {
        QJsonObject cssScheme;
        formatEntry->save(cssScheme);
        result.append(cssScheme);
    }
    json["setup"] = result;
    json["name"] = m_name;
    m_tableFormat.save(json);
}

QString FormatScheme::getName() const
{
    return m_name;
}

FormatRuleList& FormatScheme::getEntries()
{
    return m_formatRuleList;
}

bool FormatScheme::hasEntry(const QString& id)
{
    foreach (auto formatRule, m_formatRuleList)
    {
        if (id == formatRule->getModuleIdScope())
        {
            return true;
        }
    }
    return false;
}

void FormatScheme::addFormatRule(FormatRule* formatRule)
{
    m_formatRuleList.append(formatRule);
}

void FormatScheme::deleteFormatRule(FormatRule* formatRule)
{
    int i = m_formatRuleList.indexOf(formatRule);

    if (i != -1)
    {
        delete m_formatRuleList.takeAt(i);
    }
}

LogEntryFormatterPtr FormatScheme::findLogEntryFormatter(const QStringList& logMessages) const
{
    foreach (auto formatRule, m_formatRuleList)
    {
        auto logEntryFormat = formatRule->match(logMessages, m_name, m_tableFormat.getEnabledColumns());
        if (logEntryFormat)
            return logEntryFormat;
    }
    return LogEntryFormatterPtr();
}

TableFormat& FormatScheme::getTableFormat()
{
    return m_tableFormat;
}
