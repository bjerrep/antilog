#pragma once

#include "columndefinitions.h"

#include <QJsonObject>

class TableCellFormat
{
public:
    TableCellFormat();
    TableCellFormat(const Column& column, const QString& html1, const QString& html2, int width);
    TableCellFormat(const QJsonObject& json);

    void save(QJsonObject& json) const;

    QString getHtmlStart() const;
    QString getHtmlEnd() const;

    Column::ColumnType getType() const;
    QString getTypeName() const;
    QString getName() const;

    bool operator == (const TableCellFormat& other) const;
    bool isEnabled() const;
    int pixelWidth() const;

private:
    Column m_column;
    QString m_html1;
    QString m_html2;
    int m_length;
    bool m_enabled = true;

    friend class TableFormat;
};

using TableCellFormatList = QVector<TableCellFormat*>;
