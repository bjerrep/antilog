#include "tableformat.h"
#include "statics.h"
#include "options.h"

#include <QJsonArray>
#include <QDateTime>

// ------ TableCellFormat -------

TableCellFormat::TableCellFormat()
{
}

TableCellFormat::TableCellFormat(const QString& rowName, const QString& html1, const QString& html2, int width)
    : m_name(rowName),
      m_html1(html1),
      m_html2(html2),
      m_width(width)
{
}

TableCellFormat::TableCellFormat(const QJsonObject& json)
{
    m_name = json["name"].toString();
    m_html1 = json["html1"].toString();
    m_html2 = json["html2"].toString();
    m_enabled = json["enabled"].toBool();
    m_width = json["width"].toInt();
}

void TableCellFormat::save(QJsonObject& json) const
{
    json["name"] = m_name;
    json["html1"] = m_html1;
    json["html2"] = m_html2;
    json["enabled"] = m_enabled;
    json["width"] = m_width;
}

QString TableCellFormat::getHtmlStart() const
{
    return m_width ? m_html1.arg(m_width) : m_html1;
}

bool TableCellFormat::operator == (const TableCellFormat& other) const
{
    return other.m_name == m_name;
}


// ------ TableFormat -------

TableFormat::TableFormat(const QJsonObject& json)
{
    load(json);
}

void TableFormat::save(QJsonObject& json) const
{
    QJsonArray jsonArray;
    foreach (auto rowFormat, m_cellFormats)
    {
        QJsonObject obj;
        rowFormat.save(obj);
        jsonArray.append(obj);
    }
    json["tablecellformats"] = jsonArray;
}

void TableFormat::load(const QJsonObject& json)
{
    m_cellFormats.clear();
    if (json.empty())
    {
        constructDefaultSetup(Statics::options->logFontMetrics());
        return;
    }
    auto jsonArray = json["tablecellformats"].toArray();
    foreach (auto rowValue, jsonArray)
    {
        auto jsonRow = rowValue.toObject();
        m_cellFormats.append(TableCellFormat(jsonRow));
    }
    findLogLevelIndex();
}

int TableFormat::nofColumns() const
{
    return m_cellFormats.size();
}

int TableFormat::nofEnabledColumns() const
{
    int count = 0;
    foreach (auto rowFormat, m_cellFormats)
    {
        if (rowFormat.isEnabled())
            count++;
    }
    return count;
}

QStringList TableFormat::getEnabledColumns() const
{
    QStringList ret;
    foreach (auto cellFormat, m_cellFormats)
    {
        if (cellFormat.isEnabled())
        {
            ret << cellFormat.getName();
        }
    }
    return ret;
}

QVector<TableCellFormat> const& TableFormat::getCells() const
{
    return m_cellFormats;
}

int TableFormat::getLogLevelIndex()
{
    return m_logLevelIndex;
}

void TableFormat::findLogLevelIndex()
{
    m_logLevelIndex = 0;
    foreach (auto cellFormat, m_cellFormats)
    {
        if (cellFormat.getName() == Statics::Level)
        {
            return;
        }
        ++m_logLevelIndex;
    }
}

void TableFormat::setActiveCells(const QStringList& names)
{
    TableCellFormatList cellFormatCopy = m_cellFormats;
    m_cellFormats.clear();

    foreach (const QString& name, names)
    {
        foreach (auto cell, cellFormatCopy)
        {
            if (cell.getName() == name)
            {
                TableCellFormat tempCell = cell;
                tempCell.m_enabled = true;
                m_cellFormats.append(tempCell);
                cellFormatCopy.removeAll(cell);
                continue;
            }
        }
    }

    foreach (auto cell, cellFormatCopy)
    {
        TableCellFormat tempCell = cell;
        tempCell.m_enabled = false;
        m_cellFormats.append(tempCell);
    }
    findLogLevelIndex();
}

QString TableFormat::getHtml(const QString& text, int row) const
{
    return m_cellFormats.at(row).getHtmlStart() + text + m_cellFormats.at(row).getHtmlEnd();
}

QString TableFormat::getEntryCellsAsHtml(const QStringList& cells)
{
    QString html;
    auto cellFormat_it = m_cellFormats.cbegin();
    foreach (auto cell, cells)
    {
        html += cellFormat_it->getHtmlStart() + cell + cellFormat_it->getHtmlEnd();
        if (cellFormat_it == m_cellFormats.cend())
        {
            return html;
        }
        ++cellFormat_it;
    }
    return html;
}

void TableFormat::add(const QString& name, const QString& html1, const QString& html2, int width)
{
    m_cellFormats.append(TableCellFormat(name, html1, html2, width));
}

void TableFormat::recalculate(const QFontMetrics& fontMetrics)
{
    // missing
}

void TableFormat::constructDefaultSetup(const QFontMetrics& fontMetrics)
{
    const int space = 10;
    m_cellFormats.clear();

    foreach (auto cell, Statics::options->m_cellNames.m_cells)
    {
        const QString& name = cell.m_name;

        if (name == Statics::Date)
        {
            int w = space + fontMetrics.width("0000-00-00");
            add(Statics::Date, "<td width=%1>", "</td>", w);
        }
        else if (name == Statics::Time)
        {
            int w = space + fontMetrics.width("00:00:00:000");
            add(Statics::Time, "<td width=%1>", "</td>", w);
        }
        else if (name == Statics::Level)
        {
            int w = space + fontMetrics.width("warning");
            add(Statics::Level, "<td width=%1>", "</td>", w);
        }
        else if (name == Statics::Id)
        {
            int w = space + fontMetrics.width("console");
            add(Statics::Id, "<td width=%1>", "</td>", w);
        }
        else if (name == Statics::Message)
        {
            add(Statics::Message, "<td>", "</td>", 0);
        }
        else
        {
            if (cell.m_width)
            {
                add(cell.m_name, "<td width=%1>", "</td>", cell.m_width);
            }
            else
            {
                add(cell.m_name, "<td>", "</td>", 0);
            }
        }
    }
    findLogLevelIndex();
}
