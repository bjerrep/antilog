#include "logentry.h"
#include "logseverities.h"
#include "format/formatscheme.h"
#include "options.h"
#include "statics.h"

#include <QVariant>
#include <QSharedPointer>
#include <QTextDocument>


int LogEntry::s_staticSerial = 0;

LogEntry::LogEntry(const QString& cell, const QString& sourceName, FormatScheme* formatScheme)
    : m_formatScheme(formatScheme),
      m_sourceName(sourceName),
      m_cells(cell)
{
}

LogEntry::LogEntry(const QStringList& cells, const QString& sourceName, FormatScheme* formatScheme)
    : m_formatScheme(formatScheme),
      m_sourceName(sourceName),
      m_cells(cells)
{
    int logSeverityIndex = formatScheme->getColumnSetup().getLogSeverityIndex();
    if (logSeverityIndex >= 0 && logSeverityIndex < cells.count())
    {
        determineLogSeverity(cells.at(logSeverityIndex));
    }
}

LogEntry::~LogEntry()
{
}

void LogEntry::determineLogSeverity(QString logSeverityString)
{
    m_severity = Statics::s_logSeverities->findLogSeverity(logSeverityString);
}

bool LogEntry::isInScope(const QString& severity, const QString& textFilter) const
{
    int severityValue = Statics::s_logSeverities->value(severity);
    int currentSeverityValue = Statics::s_logSeverities->value(m_severity);
    bool active = severityValue <= currentSeverityValue;

    if (active && textFilter != Statics::LogSeverityFilterOff)
    {
        foreach (const QString& cell, m_cells)
        {
            if (cell.contains(textFilter))
            {
                return true;
            }
        }
        return false;
    }
    return active;
}

int LogEntry::getWidth() const
{
    if (m_width == -1)
    {
        invalidateCachedHtml();
        getHtml();
    }
    return m_width;
}

QString LogEntry::getHtml() const
{
    if (!m_htmlCached.isEmpty())
    {
        return m_htmlCached;
    }

    if (Statics::instOptions()->m_showSource)
        m_htmlCached = m_formatScheme->getTableRowHtml(m_cells, m_sourceName);
    else
        m_htmlCached = m_formatScheme->getTableRowHtml(m_cells);

    QTextDocument doc;
    doc.setHtml(m_htmlCached);
    m_width = doc.idealWidth();
    return m_htmlCached;
}

QString LogEntry::getText() const
{
    if (Statics::instOptions()->m_showSource)
        return m_formatScheme->getTableRowText(m_cells, m_sourceName);
    else
        return m_formatScheme->getTableRowText(m_cells);
}

QString LogEntry::getSourceName() const
{
    return m_sourceName;
}

QString LogEntry::getModuleName() const
{
    int index = m_formatScheme->getColumnSetup().getModuleIdIndex();

    if (index != SchemeColumnModel::IndexNotFound && index < m_cells.size())
    {
        return m_cells[index];
    }

    return "N/A";
}

QString LogEntry::toString() const
{
    return
            "Module: " + getModuleName() + "\n" +
            "Source: " + getSourceName() + "\n" +
            "FormatScheme: " + m_formatScheme->getName();
}

void LogEntry::invalidateCachedHtml() const
{
    m_width = -1;
    m_htmlCached.clear();
}

int LogEntry::getSerial() const
{
    return m_serial;
}
