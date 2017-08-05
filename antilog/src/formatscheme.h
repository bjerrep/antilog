#pragma once

#include "logentryformatter.h"
#include "tableformat.h"

#include <QJsonObject>
#include <QVector>

class FormatRule;

///
/// The FormatScheme is a named collection of FormatRule objects.
/// The None formatscheme is always added at program start. This is non editable
/// and contains no FormatEntry objects.
///
using FormatRuleList = QVector<FormatRule*>;

class FormatScheme
{
public:
    FormatScheme(const QJsonObject& json);
    FormatScheme(const QString& name);

    void save(QJsonObject& json) const;
    QString name() const;
    FormatRuleList& getEntries();
    bool hasEntry(const QString& id);
    void addFormatRule(FormatRule* formatRule);
    void deleteFormatRule(FormatRule* formatRule);
    LogEntryFormatterPtr findLogEntryFormatter(const QStringList& data) const;
    TableFormat& getTableFormat();

private:
    FormatRuleList m_formatRules;
    QString m_name;
    TableFormat m_tableFormat;

    friend class FormatDialog;
};
