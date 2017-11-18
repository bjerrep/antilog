#pragma once

#include "logentryformatter.h"
#include "tableformat.h"

#include <QJsonObject>
#include <QVector>

class FormatRule;

using FormatRuleList = QVector<FormatRule*>;

///
/// The FormatScheme is a named collection of FormatRule objects.
/// The None formatscheme is always added at program start. This is non editable
/// and contains no FormatEntry objects.
///
class FormatScheme
{
public:
    FormatScheme(const QJsonObject& json);
    FormatScheme(const QString& getName);
    ~FormatScheme();

    void save(QJsonObject& json) const;
    QString getName() const;
    FormatRuleList& getEntries();
    bool hasEntry(const QString& id);
    void addFormatRule(FormatRule* formatRule);
    void deleteFormatRule(FormatRule* formatRule);
    LogEntryFormatterPtr findLogEntryFormatter(const QStringList& data) const;
    TableFormat& getTableFormat();

private:
    FormatRuleList m_formatRuleList;
    QString m_name;
    TableFormat m_tableFormat;

    friend class FormatDialog;
};
