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

    QStringList textsAsHtml(const QStringList& texts) const;
    QString getStylesheets() const;
    void addLogCellFormatter(LogCellFormatter logCellFormatter);
    QString formatSchemeName() const;

private:
    QList<LogCellFormatter> m_logCellFormatterList;
    QString m_formatschemeName;
};

using LogEntryFormatterPtr = QSharedPointer<LogEntryFormatter>;
