#pragma once

#include <QString>
#include <QVector>

/// The LogCellFormatter can html format a log cell. A list of LogCellFormatters
/// are owned by a LogEntryFormatter. (see also FormatRule which construct the formatters)
///
class LogCellFormatter
{
public:
    LogCellFormatter(const QString& name);

    QString getCellHtml(QString logMessage) const;
    QString getStylesheet() const;
    void wordMode(const QString& word, bool caseSensitive);
    void setBold();
    void setColor(const QString& colorName);
    void setBackgroundColor(const QString& color);

private:
    QString m_name;
    QString m_preHtml;
    QString m_postHtml;
    QString m_styleSheet;
    QVector<int> m_wordIndexes;
    int m_length = -1;
};
