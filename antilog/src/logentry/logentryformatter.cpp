#include "logentryformatter.h"
#include "logcellformatter.h"
#include "statics.h"

LogEntryFormatter::LogEntryFormatter(const QString& formatschemeName, const QStringList& names)
    : m_formatschemeName(formatschemeName)
{
    foreach (auto name, names)
    {
        m_logCellFormatterList.append(LogCellFormatter(name));
    }
}

LogEntryFormatter::LogEntryFormatter(const QString& formatschemeName)
    : m_formatschemeName(formatschemeName)
{
}

QStringList LogEntryFormatter::textsAsHtml(const QStringList& texts) const
{
    QStringList html;

    for(int i = 0; i < texts.size() && i < m_logCellFormatterList.size(); i++)
    {
        html.append(m_logCellFormatterList.at(i).getCellHtml(texts.at(i)));
    }
    return html;
}

QString LogEntryFormatter::getStylesheets() const
{
    QString css;
    foreach (auto logCellFormat, m_logCellFormatterList)
    {
        css += logCellFormat.getStylesheet();
    }
    return css;
}

void LogEntryFormatter::addLogCellFormatter(LogCellFormatter logCellFormatter)
{
    m_logCellFormatterList.append(logCellFormatter);
}

QString LogEntryFormatter::formatSchemeName() const
{
    return m_formatschemeName;
}
