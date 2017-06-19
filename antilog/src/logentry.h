#pragma once

#include "tableformat.h"
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
    LogCell(const QString& key, const QString& text);

    const QString m_key;
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

    void determineLogLevel(QString logLevelData);
    bool isInScope(const QString& level, const QString& textFilter) const;
    QStringList getEntriesList() const;
    int getNofRows() const;
    QString getHtml();
    int getSerial() const;
    void invalidateCachedHtml();

private:
    static int staticSerial;
    int m_serial = LogEntry::staticSerial++;
    QVector<LogCell> m_logCells;
    QString m_level = Statics::logLevelFilterOff;
    FormatScheme* m_formatScheme = nullptr;
    QString m_htmlCached;
    QString m_sourceName;
};

using LogEntryPtr = QSharedPointer<LogEntry>;

Q_DECLARE_METATYPE(LogEntryPtr)