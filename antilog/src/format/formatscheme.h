#pragma once

#include "formatrule.h"
#include "column/schemecolumnmodel.h"

#include <QJsonObject>
#include <QVector>

class FormatRule;
class FormatSchemes;

/// The FormatScheme is a named collection of FormatRule objects.
/// The None formatscheme is always added at program start. This is non editable
/// and contains no FormatEntry objects.
///
class FormatScheme
{
public:
    FormatScheme(const QJsonObject& json, FormatSchemes* parentModel);
    FormatScheme(const QString& getName, FormatSchemes* parentModel);
    ~FormatScheme();

    void save(QJsonObject& json) const;
    QString getName() const;
    FormatRuleList& getEntries();
    bool hasEntry(const QString& id);
    void addFormatRule(FormatRule* formatRule);
    void deleteFormatRule(FormatRule* formatRule);

    QString tableHtml(const QStringList& logCells,
                      const QString& sourceData,
                      const FormatRuleList& formatRuleList,
                      const GlobalColumnTypeList& columnTypeList,
                      const GlobalColumnConfig* tableFormat);

    QString tableText(const QStringList& logCells,
                      const QString& sourceData,
                      const GlobalColumnTypeList& columnTypeList);

    QString getTableRowHtml(const QStringList& logEntries, QString sourceName = QString());
    QString getTableRowText(const QStringList& logEntries, QString sourceName = QString());
    SchemeColumnModel& getColumnSetup();

private:
    FormatRuleList m_formatRuleList;
    QString m_name;
    SchemeColumnModel* m_schemeColumnModel;
    FormatSchemes* m_parent;

    friend class FormatDialog;
};
