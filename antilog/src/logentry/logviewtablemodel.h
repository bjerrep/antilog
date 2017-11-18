#pragma once

#include "logentry.h"
#include "statics.h"

#include <QStyledItemDelegate>
#include <QModelIndex>

class TableCellFormat;
class ExtendedFilterModel;
class QPainter;

class LogViewDelegate : public QStyledItemDelegate
{
public:
    LogViewDelegate(QObject* parent = nullptr);
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const;
};


class LogViewTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    LogViewTableModel(QObject* parent, const ExtendedFilterModel* filterModel);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void clear();
    void erase(int count);
    bool logEntryIsVisible(LogEntryPtr logEntry);
    void append(LogEntryPtr logEntry);
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());
    void append(QVector<LogEntryPtr> logEntries);
    void timerEvent(QTimerEvent*);
    void newLogSeverity(const QString& severity);
    void setTextFilter(const QString& contains);
    void redrawVisibleRows();

signals:
    void newEntriesAdded();
    void signalDeletingRows(int count);

private slots:
    void slotRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);

private:
    QVector<LogEntryPtr> m_logEntries;
    QVector<LogEntryPtr> m_logEntriesVisible;
    QVector<LogEntryPtr> m_logEntriesPending;
    QString m_logSeverity;
    QString m_textFilter = Statics::LogSeverityFilterOff;
    const ExtendedFilterModel* m_extendedFilterModel;
};

