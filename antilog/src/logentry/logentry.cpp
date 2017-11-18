#include "logentry.h"
#include "logseverities.h"
#include "logentryformatter.h"
#include "formatscheme.h"
#include "tableformat.h"
#include "options.h"
#include "statics.h"

#include <QVariant>
#include <QSharedPointer>
#include <QTextDocument>

const int MAX_SOURCENAME_WIDTH(17);

// ------ LogCell -------

LogCell::LogCell()
    : m_text(QString())
{
}

LogCell::LogCell(Column::ColumnType key, const QString& text)
    : m_key(key),
      m_text(text)
{
}

// ------ LogEntry -------

int LogEntry::s_staticSerial = 0;
int LogEntry::s_sourceFieldWidth = 0;

LogEntry::LogEntry(const QString& message, const QString& sourceName, FormatScheme* formatScheme)
    : m_formatScheme(formatScheme),
      m_sourceName(sourceName)
{
    m_logCells.append(LogCell(Column::TEXT, message));
}

LogEntry::LogEntry(const QStringList& texts, const QString& sourceName, FormatScheme* formatScheme)
    : m_formatScheme(formatScheme),
      m_sourceName(sourceName)
{
    ColumnTypeList rows = formatScheme->getTableFormat().getEnabledColumns();

    int loops = qMin(texts.size(), rows.size());

    for (int i = 0; i < loops; i++)
    {
        m_logCells.append(LogCell(rows.at(i), texts.at(i)));
    }
    int logSeverityIndex = formatScheme->getTableFormat().getLogSeverityIndex();
    if (logSeverityIndex >= 0 && logSeverityIndex < texts.count())
    {
        determineLogSeverity(texts.at(logSeverityIndex));
    }
}

LogEntry::~LogEntry()
{
    s_sourceFieldWidth = 0;
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
        foreach (auto&& logCell, m_logCells)
        {
            if (logCell.m_text.contains(textFilter))
            {
                return true;
            }
        }
        return false;
    }
    return active;
}

QStringList LogEntry::getEntriesList() const
{
    QStringList ret;
    foreach (auto logCell, m_logCells)
    {
        ret << logCell.m_text;
    }
    return ret;
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

    QStringList subMessages = getEntriesList();
    QString css;
    LogEntryFormatterPtr logEntryFormatter = m_formatScheme->findLogEntryFormatter(subMessages);

    if (logEntryFormatter)
    {
        subMessages = logEntryFormatter->textsAsHtml(subMessages);
        css = logEntryFormatter->getStylesheets();
    }

    QString source;
    if (Statics::getOptions()->m_showSource)
    {
        auto sourceName = m_sourceName.left(MAX_SOURCENAME_WIDTH);
        const int sourceLength = sourceName.size();
        if (sourceLength > s_sourceFieldWidth)
        {
            s_sourceFieldWidth = sourceLength;
        }
        int widthInPixels = Statics::getOptions()->logFontWidth(s_sourceFieldWidth + Statics::getOptions()->m_logViewSpacing);
        source = QString("<td width=%1><b><small>%2</small></b></td>").arg(widthInPixels).arg(sourceName);
    }
    QString html = source + m_formatScheme->getTableFormat().getEntryCellsAsHtml(subMessages);
    m_htmlCached = "<html>" + css + "<table><tr>" + html + "</tr></table></html>";

    QTextDocument doc;
    doc.setHtml(m_htmlCached);
    m_width = doc.idealWidth();
    return m_htmlCached;
}

QString LogEntry::getText() const
{
    QString source;
    if (Statics::getOptions()->m_showSource)
    {
        source = m_sourceName.left(s_sourceFieldWidth).leftJustified(s_sourceFieldWidth, ' ') + " - ";
    }
    return source + m_formatScheme->getTableFormat().getEntryCellsAsText(getEntriesList());
}

QString LogEntry::getSourceName() const
{
    return m_sourceName;
}

QString LogEntry::getModuleName() const
{
    foreach (auto logCell, m_logCells)
    {
        if (logCell.m_key == Column::MODULEID)
        {
            return logCell.m_text;
        }
    }
    return QString();
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
