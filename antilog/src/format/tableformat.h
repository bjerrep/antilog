#pragma once

#include "tablecellformat.h"
#include "columndefinitions.h"

#include <QJsonObject>
#include <QVector>
#include <QFontMetrics>


/// The TableFormat is part of a FormatScheme and knows how to generate
/// a html formatted row for the log view.
///
class TableFormat : public QObject
{
    Q_OBJECT

public:
    TableFormat(const QJsonObject& json = QJsonObject());
    ~TableFormat();

    void save(QJsonObject& json) const;

    int nofEnabledColumns() const;
    const ColumnTypeList& getEnabledColumns() const;
    int getNofEnabledColumns() const;
    QStringList getEnabledColumnsStringList() const;
    TableCellFormatList const& getTableCellFormatList() const;
    QString getHtml(const QString& text, int column) const;
    QString getText(const QString& text, int column) const;
    QString getEntryCellsAsHtml(const QStringList& texts);
    QString getEntryCellsAsText(const QStringList& texts);
    int getLogSeverityIndex();
    void setActiveCells(const QStringList& names);
    void recalculate(const QFontMetrics& fontMetrics);

    const int NoSeverityExist = -1;

private slots:
    void slotColumnsInvalidated();

private:
    void invalidated();
    void load(const QJsonObject& json);
    void findLogSeverityIndex();
    void add(const Column& Column, const QString& html1, const QString& html2, int width);
    void constructDefaultSetup(const QFontMetrics& fontMetrics);

private:
    TableCellFormatList m_tableCellFormatList;
    ColumnTypeList m_enabledColumnTypeList;
    int m_logSeverityIndex = NoSeverityExist;
};
