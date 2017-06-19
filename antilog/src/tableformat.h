#pragma once

#include <QJsonObject>
#include <QVector>
#include <QFontMetrics>


// ------ TableCellFormat -------

class TableCellFormat
{
public:
    TableCellFormat();
    TableCellFormat(const QString& rowName, const QString& html1, const QString& html2, int width);
    TableCellFormat(const QJsonObject& json);

    void save(QJsonObject& json) const;

    QString getHtmlStart() const;
    QString getHtmlEnd() const { return m_html2; }
    QString getName() const { return m_name; }
    bool operator == (const TableCellFormat& other) const;
    bool isEnabled() const { return m_enabled; }

private:
    QString m_name;
    QString m_html1;
    QString m_html2;
    int m_width;
    bool m_enabled = true;

    friend class TableFormat;
};

// ------ TableFormat -------

using TableCellFormatList = QVector<TableCellFormat>;

/// The TableFormat is part of a FormatScheme and knows how to generate
/// a html formatted tablerow for the log view.
///
class TableFormat
{
public:
    TableFormat(const QJsonObject& json = QJsonObject());
    void save(QJsonObject& json) const;
    void load(const QJsonObject& json);

    int nofColumns() const;
    int nofEnabledColumns() const;
    QStringList getEnabledColumns() const;
    TableCellFormatList const& getCells() const;
    QString getHtml(const QString& text, int row) const;
    QString getEntryCellsAsHtml(const QStringList& texts);
    int getLogLevelIndex();
    void setActiveCells(const QStringList& names);
    void recalculate(const QFontMetrics& fontMetrics);

private:
    void findLogLevelIndex();
    void add(const QString& rowname, const QString& html1, const QString& html2, int width);
    void constructDefaultSetup(const QFontMetrics& fontMetrics);

private:
    TableCellFormatList m_cellFormats;
    int m_logLevelIndex = 0;
};
