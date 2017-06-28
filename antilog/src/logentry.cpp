#include "logentry.h"
#include "loglevels.h"
#include "logentryformatter.h"
#include "formatscheme.h"
#include "tableformat.h"
#include "options.h"
#include "statics.h"

#include <QVariant>
#include <QSharedPointer>

// ------ LogCell -------

LogCell::LogCell()
    : m_text(QString())
{
}

LogCell::LogCell(const QString& key, const QString& text)
    : m_key(key),
      m_text(text)
{
}

// ------ LogEntry -------

int LogEntry::staticSerial = 0;

LogEntry::LogEntry(const QString& message, const QString& sourceName, FormatScheme* formatScheme)
    : m_formatScheme(formatScheme),
      m_sourceName(sourceName)
{
    m_logCells.append(LogCell(Statics::Message, message));
}

LogEntry::LogEntry(const QStringList& texts, const QString& sourceName, FormatScheme* formatScheme)
    : m_formatScheme(formatScheme),
      m_sourceName(sourceName)
{
    const QStringList& rows = formatScheme->getTableFormat().getEnabledColumns();
    int loops = qMin(texts.size(), rows.size());
    for (int i = 0; i < loops; i++)
    {
        m_logCells.append(LogCell(rows.at(i), texts.at(i)));
    }
    int logLevelIndex = formatScheme->getTableFormat().getLogLevelIndex();
    determineLogLevel(texts.at(logLevelIndex));
}

void LogEntry::determineLogLevel(QString logLevelData)
{
    m_level = Statics::logLevels->findLogLevel(logLevelData);
}

bool LogEntry::isInScope(const QString& level, const QString& textFilter) const
{
    int _level = Statics::logLevels->value(level);
    int mlevel = Statics::logLevels->value(m_level);
    bool active = _level <= mlevel;

    if (active && textFilter != Statics::logLevelFilterOff)
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

int LogEntry::getNofRows() const
{
    return m_logCells.size();
}

QString LogEntry::getHtml()
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
    if (Statics::options->m_showSource)
    {
        const int sourceLength = 18;
        int widthInPixels = Statics::options->logFontWidth(sourceLength + Statics::options->m_logViewSpacing);
        source = QString("<td width=%1>").arg(widthInPixels) +
                 m_sourceName.left(sourceLength).rightJustified(sourceLength, ' ') + "</td>";
    }
    QString html = source + m_formatScheme->getTableFormat().getEntryCellsAsHtml(subMessages);
    return m_htmlCached = "<html>" + css + "<table><tr>" + html + "</tr></table></html>";
}

QString LogEntry::getText() const
{
    return m_formatScheme->getTableFormat().getEntryCellsAsText(getEntriesList());
}

void LogEntry::invalidateCachedHtml()
{
    m_htmlCached.clear();
}

int LogEntry::getSerial() const
{
    return m_serial;
}
