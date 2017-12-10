#pragma once

#include "globalcolumn.h"

#include <QJsonObject>
#include <QVector>
#include <QFontMetrics>
#include <QMap>

using TableCellFormatMap = QMap<GlobalColumn::ColumnType, GlobalColumn*>;

/// Contains the list of all available GlobalColumn definitions.
/// It is used when constructing the per instance SchemeColumnList for new FormatScheme objects.
///
class GlobalColumnConfig : public QObject
{
    Q_OBJECT

public:
    GlobalColumnConfig(const QJsonObject& json);
    ~GlobalColumnConfig();

    void save(QJsonObject& json) const;

    TableCellFormatMap const& getTableCellFormatMap() const;
    QString getTableDataHtml(const QString& text, GlobalColumn::ColumnType tpe) const;
    int getLogSeverityIndex() const;
    QString getName(GlobalColumn::ColumnType tpe) const;
    QStringList getColumnTypesAsStringlist() const;
    void refreshColumns(GlobalColumnList& columnFormatList);
    void constructDefaultSetup();

private:
    void load(const QJsonObject& json);
    void add(GlobalColumn::ColumnType columnType,
             const QString& name,
             const QString& html1,
             const QString& html2,
             int width);

private:
    TableCellFormatMap m_tableCellFormatMap;
};
