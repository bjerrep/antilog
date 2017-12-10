#pragma once

#include "schemecolumn.h"
#include "statics.h"

#include <QObject>
#include <QString>
#include <QVector>
#include <QSharedPointer>

class FormatScheme;


// A LogEntry is constructed by a processor with a single message
// or a group of cells from splitting the original message.
// The processor also defines what formatting scheme the logentry should use.
//
class LogEntry
{
public:
    LogEntry(const QString& cell, const QString& sourceName, FormatScheme* formatScheme = nullptr);
    LogEntry(const QStringList& cells, const QString& sourceName, FormatScheme* formatScheme = nullptr);
    ~LogEntry();

    void determineLogSeverity(QString logSeverityString);
    bool isInScope(const QString& severity, const QString& textFilter) const;
    int getWidth() const;
    QString getHtml() const;
    QString getText() const;
    int getSerial() const;
    void invalidateCachedHtml() const;
    QString getSourceName() const;
    QString getModuleName() const;
    QString toString() const;

private:
    static int s_staticSerial;
    int m_serial = LogEntry::s_staticSerial++;
    QString m_severity = Statics::LogSeverityFilterOff;
    FormatScheme* m_formatScheme = nullptr;
    QString m_sourceName;
    mutable QString m_htmlCached;
    mutable int m_width = -1;
    QStringList m_cells;
};

using LogEntryPtr = QSharedPointer<LogEntry>;

Q_DECLARE_METATYPE(LogEntryPtr)
