#pragma once
#include "logcellformatter.h"
#include "tableformat.h"
#include <QJsonArray>
#include <QSharedPointer>

class LogEntryFormatter
{
public:
    LogEntryFormatter(const QString& formatschemeName, const QStringList& names);
    LogEntryFormatter(const QString& formatschemeName);

    const LogCellFormatter& getItemScheme(int column) const;
    QString logStringsAsHtml(const QStringList& texts, const TableFormat& tableFormat);
    QStringList textsAsHtml(const QStringList& texts) const;
    QString getStylesheets() const;
    void setEditable(bool editable);
    void addLogCellFormatter(LogCellFormatter logCellFormatter);
    QString formatSchemeName() const;

private:
    bool m_editable = true;
    QList<LogCellFormatter> m_logCellFormats;
    QString m_formatschemeName;
};

using LogEntryFormatterPtr = QSharedPointer<LogEntryFormatter>;
