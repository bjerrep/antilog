#include "globalcolumnconfig.h"
#include "schemecolumn.h"
#include "options.h"
#include "statics.h"

#include <QObject>
#include <QJsonArray>
#include <QDateTime>
#include <QMetaEnum>

GlobalColumnConfig::GlobalColumnConfig(const QJsonObject& json)
{
    load(json);
}

GlobalColumnConfig::~GlobalColumnConfig()
{
    qDeleteAll(m_tableCellFormatMap);
}

void GlobalColumnConfig::save(QJsonObject& json) const
{
    QJsonArray jsonArray;
    foreach (auto rowFormat, m_tableCellFormatMap)
    {
        QJsonObject obj;
        rowFormat->save(obj);
        jsonArray.append(obj);
    }
    json["globalcolumnconfig"] = jsonArray;
}

const TableCellFormatMap &GlobalColumnConfig::getTableCellFormatMap() const
{
    return m_tableCellFormatMap;
}

void GlobalColumnConfig::load(const QJsonObject& json)
{
    if (json.empty())
    {
        constructDefaultSetup();
        return;
    }

    auto jsonArray = json["globalcolumnconfig"].toArray();
    foreach (auto rowValue, jsonArray)
    {
        auto jsonRow = rowValue.toObject();
        auto globalColumn = new GlobalColumn(jsonRow);
        m_tableCellFormatMap.insert(globalColumn->getType(), globalColumn);
    }
}

QString GlobalColumnConfig::getTableDataHtml(const QString& text, GlobalColumn::ColumnType colType) const
{
    if (colType == GlobalColumn::UNKNOWN || !m_tableCellFormatMap.contains(colType))
    {
        return "<td>" + text + "</td>";
    }

    return m_tableCellFormatMap[colType]->getHtmlTableDataStart() + text + m_tableCellFormatMap[colType]->getHtmlTableDataEnd();
}

QString GlobalColumnConfig::getName(GlobalColumn::ColumnType tpe) const
{
    if (m_tableCellFormatMap.contains(tpe))
    {
        return m_tableCellFormatMap[tpe]->getName();
    }
    return QString();
}

QStringList GlobalColumnConfig::getColumnTypesAsStringlist() const
{
    QStringList columnTypes;

    int index = GlobalColumn::staticMetaObject.indexOfEnumerator("ColumnType");
    auto meta = GlobalColumn::staticMetaObject.enumerator(index);

    for (int i = 0; i < meta.keyCount(); i++)
    {
        columnTypes << meta.key(i);
    }
    return columnTypes;
}

void GlobalColumnConfig::refreshColumns(GlobalColumnList& columnFormatList)
{
    m_tableCellFormatMap.clear();

    for (auto tablecell : columnFormatList)
    {
           m_tableCellFormatMap.insert(tablecell->getType(), tablecell);
    }
}

void GlobalColumnConfig::add(GlobalColumn::ColumnType columnType,
                             const QString& name,
                             const QString& html1,
                             const QString& html2,
                             int width)
{
   m_tableCellFormatMap.insert(columnType, new GlobalColumn(columnType, name, html1, html2, width));

}

void GlobalColumnConfig::constructDefaultSetup()
{
    const int space = Statics::instOptions()->m_logViewSpacing;
    m_tableCellFormatMap.clear();

    add(GlobalColumn::DATE, Statics::DefaultDateName, "<td width=%1>", "</td>", space + strlen("0000-00-00"));
    add(GlobalColumn::TIME, Statics::DefaultTimeName, "<td width=%1>", "</td>", space + strlen("00:00:00:000"));
    add(GlobalColumn::SEVERITY, Statics::DefaultSeverityName, "<td width=%1>", "</td>", space + strlen("warning"));
    add(GlobalColumn::MODULEID, Statics::DefaultModuleIdName, "<td width=%1>", "</td>", space + strlen("console"));
    add(GlobalColumn::TEXT, Statics::DefaultTextName, "<td>", "</td>", 0);
}
