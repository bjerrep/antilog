#include "tableformat.h"
#include "columndefinitions.h"
#include "options.h"
#include "statics.h"

#include <QObject>
#include <QJsonArray>
#include <QDateTime>


TableFormat::TableFormat(const QJsonObject& json)
{
    load(json);

    connect(Statics::getOptions()->m_columnDefinitions, &ColumnDefinitions::signalInvalidated,
            this, &TableFormat::slotColumnsInvalidated);
}

TableFormat::~TableFormat()
{
    qDeleteAll(m_tableCellFormatList);
    //qDeleteAll(m_enabledTableCellFormatList);
}

void TableFormat::save(QJsonObject& json) const
{
    QJsonArray jsonArray;
    foreach (auto rowFormat, m_tableCellFormatList)
    {
        QJsonObject obj;
        rowFormat->save(obj);
        jsonArray.append(obj);
    }
    json["tablecellformats"] = jsonArray;
}

void TableFormat::load(const QJsonObject& json)
{
    if (json.empty())
    {
        constructDefaultSetup(Statics::getOptions()->logFontMetrics());
        return;
    }
    auto jsonArray = json["tablecellformats"].toArray();
    foreach (auto rowValue, jsonArray)
    {
        auto jsonRow = rowValue.toObject();
        m_tableCellFormatList.append(new TableCellFormat(jsonRow));
    }

    invalidated();
    findLogSeverityIndex();
}

int TableFormat::nofEnabledColumns() const
{
    int count = 0;
    foreach (auto rowFormat, m_tableCellFormatList)
    {
        if (rowFormat->isEnabled())
            count++;
    }
    return count;
}

const ColumnTypeList& TableFormat::getEnabledColumns() const
{
    return m_enabledColumnTypeList;
}

int TableFormat::getNofEnabledColumns() const
{
    return m_enabledColumnTypeList.size();
}

QStringList TableFormat::getEnabledColumnsStringList() const
{
    QStringList ret;
    foreach (auto cellFormat, m_tableCellFormatList)
    {
        if (cellFormat->isEnabled())
        {
            ret << cellFormat->getTypeName();
        }
    }
    return ret;
}

TableCellFormatList const& TableFormat::getTableCellFormatList() const
{
    return m_tableCellFormatList;
}

int TableFormat::getLogSeverityIndex()
{
    return m_logSeverityIndex;
}

void TableFormat::findLogSeverityIndex()
{
    m_logSeverityIndex = 0;
    foreach (auto cellFormat, m_tableCellFormatList)
    {
        if (cellFormat->m_enabled && cellFormat->getType() == Column::SEVERITY)
        {
            return;
        }
        ++m_logSeverityIndex;
    }
    m_logSeverityIndex = NoSeverityExist;
}

void TableFormat::setActiveCells(const QStringList& names)
{
    // trying to do two things, update the enabled flags and reorder to match the
    // order in names
    TableCellFormatList cellFormatCopy = m_tableCellFormatList;
    m_tableCellFormatList.clear();

    foreach (auto name, names)
    {
        foreach (auto cell, cellFormatCopy)
        {
            if (cell->getName() == name)
            {
                TableCellFormat* tempCell = cell;
                tempCell->m_enabled = true;
                m_tableCellFormatList.append(tempCell);
                cellFormatCopy.removeAll(cell);
                continue;
            }
        }
    }

    foreach (auto cell, cellFormatCopy)
    {
        TableCellFormat* tempCell = cell;
        tempCell->m_enabled = false;
        m_tableCellFormatList.append(tempCell);
    }

    invalidated();
    findLogSeverityIndex();
}

QString TableFormat::getHtml(const QString& text, int column) const
{
    if (column >= m_tableCellFormatList.count())
    {
        throw std::out_of_range("exception in getHtml");
    }
    return m_tableCellFormatList.at(column)->getHtmlStart() + text + m_tableCellFormatList.at(column)->getHtmlEnd();
}

QString TableFormat::getText(const QString& text, int column) const
{
    if (column >= m_tableCellFormatList.count())
    {
        throw std::out_of_range("exception in getText");
    }
    return text.leftJustified(m_tableCellFormatList.at(column)->m_length, ' ');
}

QString TableFormat::getEntryCellsAsHtml(const QStringList& cells)
{
    QString html;
    try
    {
        int i = 0;
        foreach (auto cell, cells)
        {
            html += getHtml(cell, i++);
        }
    }
    catch(std::out_of_range e)
    {
        warn(e.what());
    }
    return html;
}

QString TableFormat::getEntryCellsAsText(const QStringList& cells)
{
    QString text;
    try
    {
        int i = 0;
        foreach (auto cell, cells)
        {
            text += getText(cell, i++);
        }
    }
    catch(std::out_of_range e)
    {
        warn(e.what());
    }
    return text;
}

void TableFormat::recalculate(const QFontMetrics& /*fontMetrics*/)
{
    // missing
}

void TableFormat::slotColumnsInvalidated()
{
    warn("invalidated");
}

void TableFormat::invalidated()
{
    m_enabledColumnTypeList.clear();
    foreach (auto cellFormat, m_tableCellFormatList)
    {
        if (cellFormat->isEnabled())
        {
            m_enabledColumnTypeList << cellFormat->getType();
        }
    }

}

void TableFormat::add(const Column& cellIdentifier, const QString& html1, const QString& html2, int width)
{
    m_tableCellFormatList.append(new TableCellFormat(cellIdentifier, html1, html2, width));

}

void TableFormat::constructDefaultSetup(const QFontMetrics& /*fontMetrics*/)
{
    const int space = Statics::getOptions()->m_logViewSpacing;
    m_tableCellFormatList.clear();

    foreach (auto column, Statics::getOptions()->getColumnDefinitions()->getColumnTypeList())
    {
        switch (column->getCellType())
        {
        case Column::DATE:
        {
            add(*column, "<td width=%1>", "</td>", space + strlen("0000-00-00"));
            break;
        }
        case Column::TIME:
        {
            add(*column, "<td width=%1>", "</td>", space + strlen("00:00:00:000"));
            break;
        }
        case Column::SEVERITY:
        {
            add(*column, "<td width=%1>", "</td>", space + strlen("warning"));
            break;
        }
        case Column::MODULEID:
        {
            add(*column, "<td width=%1>", "</td>", space + strlen("console"));
            break;
        }
        case Column::TEXT:
        {
            add(*column, "<td>", "</td>", 0);
            break;
        }
        default:
        {
            add(*column, "<td>", "</td>", 0);
        }
        }
    }

    invalidated();
    findLogSeverityIndex();
}
