#include "formatscheme.h"
#include "formatrule.h"
#include "options.h"

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
        m_formatRules.append(formatEntry);
    }
}

FormatScheme::FormatScheme(const QString& name)
    : m_name(name)
{
}

void FormatScheme::save(QJsonObject& json) const
{
    QJsonArray result;
    foreach (auto formatEntry, m_formatRules)
    {
        QJsonObject csscheme;
        formatEntry->save(csscheme);
        result.append(csscheme);
    }
    json["setup"] = result;
    json["name"] = m_name;
    m_tableFormat.save(json);
}

QString FormatScheme::name() const
{
    return m_name;
}

FormatRuleList& FormatScheme::getEntries()
{
    return m_formatRules;
}

void FormatScheme::add(FormatRule* formatRule)
{
    m_formatRules.append(formatRule);
}

void FormatScheme::deleteFormatEntry(FormatRule* formatRule)
{
    int i = m_formatRules.indexOf(formatRule);

    if (i != -1)
    {
        delete m_formatRules.takeAt(i);
    }
}

LogEntryFormatterPtr FormatScheme::findLogEntryFormatter(const QStringList& logMessages) const
{
    foreach (auto formatRule, m_formatRules)
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
