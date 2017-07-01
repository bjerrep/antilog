#pragma once

#include "logentry.h"
#include "statics.h"

#include <QTextDocument>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QModelIndex>

class TableCellFormat;

class LogViewDelegate : public QStyledItemDelegate
{
public:
    LogViewDelegate(QObject *parent);
    void paint(QPainter *painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const;
};


class LogViewTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    LogViewTableModel(QObject *parent);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void clear();
    void erase(int count);
    void append(LogEntryPtr logEntry);
    void append(QVector<LogEntryPtr> logEntries);
    void timerEvent(QTimerEvent*);
    void newLogLevel(const QString& level);
    void setTextFilter(const QString& contains);
    void redrawVisibleRows();

signals:
    void newEntriesAdded();

private:
    QVector<LogEntryPtr> m_rowsTotal;
    QVector<LogEntryPtr> m_visibleRows;
    QVector<LogEntryPtr> m_rowsBuffered;
    QString m_logLevel;
    QString m_textFilter = Statics::logLevelFilterOff;
};

