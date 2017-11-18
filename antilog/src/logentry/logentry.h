#pragma once

#include "tableformat.h"
#include "columndefinitions.h"
#include "statics.h"

#include <QObject>
#include <QString>
#include <QVector>
#include <QSharedPointer>

class FormatScheme;

// Internal storage for the cell(s) in a LogEntry
//
struct LogCell
{
    LogCell();
    LogCell(Column::ColumnType key, const QString& text);

    Column::ColumnType m_key;
    const QString m_text;
};

// A LogEntry is constructed by a processor with a single message
// or a array of cells/groups from splitting the original message.
// The processor also defines what formatting scheme the logentry should use.
//
class LogEntry
{
public:
    LogEntry(const QString& message, const QString& sourceName, FormatScheme* formatScheme = nullptr);
    LogEntry(const QStringList& texts, const QString& sourceName, FormatScheme* formatScheme = nullptr);
    ~LogEntry();

    void determineLogSeverity(QString logSeverityString);
    bool isInScope(const QString& severity, const QString& textFilter) const;
    QStringList getEntriesList() const;
    int getWidth() const;
    QString getHtml() const;
    QString getText() const;
    int getSerial() const;
    void invalidateCachedHtml() const;
    QString getSourceName() const;
    QString getModuleName() const;

private:
    static int s_staticSerial;
    int m_serial = LogEntry::s_staticSerial++;
    QVector<LogCell> m_logCells;
    QString m_severity = Statics::LogSeverityFilterOff;
    FormatScheme* m_formatScheme = nullptr;
    QString m_sourceName;
    mutable QString m_htmlCached;
    mutable int m_width = -1;
    static int s_sourceFieldWidth;
};

using LogEntryPtr = QSharedPointer<LogEntry>;

Q_DECLARE_METATYPE(LogEntryPtr)
