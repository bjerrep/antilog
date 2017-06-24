#include "logentryformatter.h"
#include "logcellformatter.h"
#include "statics.h"

LogEntryFormatter::LogEntryFormatter(const QString& formatschemeName, const QStringList& names)
    : m_formatschemeName(formatschemeName)
{
    foreach (auto name, names)
    {
        m_logCellFormats.append(LogCellFormatter(name));
    }
}

LogEntryFormatter::LogEntryFormatter(const QString& formatschemeName)
    : m_formatschemeName(formatschemeName)
{
}

const LogCellFormatter& LogEntryFormatter::getItemScheme(int column) const
{
    return m_logCellFormats.at(column);
}

QStringList LogEntryFormatter::textsAsHtml(const QStringList& texts) const
{
    QStringList html;

    for(int i = 0; i < texts.size() && i < m_logCellFormats.size(); i++)
    {
        html.append(m_logCellFormats.at(i).getCellHtml(texts.at(i)));
    }
    return html;
}

QString LogEntryFormatter::getStylesheets() const
{
    QString css;
    foreach (auto logCellFormat, m_logCellFormats)
    {
        css += logCellFormat.getStylesheet();
    }
    return css;
}

void LogEntryFormatter::setEditable(bool editable)
{
    m_editable = editable;
}

void LogEntryFormatter::addLogCellFormatter(LogCellFormatter logCellFormatter)
{
    m_logCellFormats.append(logCellFormatter);
}

QString LogEntryFormatter::formatSchemeName() const
{
    return m_formatschemeName;
}
