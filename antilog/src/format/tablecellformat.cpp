#include "tablecellformat.h"
#include "statics.h"
#include "options.h"

TableCellFormat::TableCellFormat()
{
}

TableCellFormat::TableCellFormat(const Column& column,
                                 const QString& html1,
                                 const QString& html2,
                                 int length)
    : m_column(column),
      m_html1(html1),
      m_html2(html2),
      m_length(length)
{
}

TableCellFormat::TableCellFormat(const QJsonObject& json)
{
    m_column.load(json);
    m_html1 = json["html1"].toString();
    m_html2 = json["html2"].toString();
    m_enabled = json["enabled"].toBool();
    m_length = json["length"].toInt();
}

void TableCellFormat::save(QJsonObject& json) const
{
    m_column.save(json);
    json["html1"] = m_html1;
    json["html2"] = m_html2;
    json["enabled"] = m_enabled;
    json["length"] = m_length;
}

QString TableCellFormat::getHtmlStart() const
{
    return m_length ? m_html1.arg(pixelWidth()) : m_html1;
}

QString TableCellFormat::getHtmlEnd() const
{
    return m_html2;
}

Column::ColumnType TableCellFormat::getType() const
{
    return m_column.getCellType();
}

QString TableCellFormat::getTypeName() const
{
    return m_column.getCellTypeAsString();
}

QString TableCellFormat::getName() const
{
    return m_column.getName();
}

bool TableCellFormat::operator == (const TableCellFormat& other) const
{
    return other.m_column.getName() == m_column.getName();
}

bool TableCellFormat::isEnabled() const
{
    return m_enabled;
}

int TableCellFormat::pixelWidth() const
{
    return Statics::getOptions()->logFontWidth(m_length);
}
