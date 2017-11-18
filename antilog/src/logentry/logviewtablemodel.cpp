#include "logviewtablemodel.h"
#include "options.h"
#include "extendedfiltermodel.h"

#include <QTextDocument>
#include <QStyleOptionViewItem>
#include <QPainter>

// ------ LogViewDelegate -------

LogViewDelegate::LogViewDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void LogViewDelegate::paint(QPainter* painter,
                            const QStyleOptionViewItem& option,
                            const QModelIndex& index) const
{
    auto logEntryPtr = static_cast<LogEntryPtr>(index.data().value<LogEntryPtr>());

    QStyledItemDelegate::paint(painter, option, index);

    if (logEntryPtr)
    {
        QTextDocument doc;
        QString html = logEntryPtr->getHtml();
        doc.setHtml(html);
        painter->save();
        painter->translate(option.rect.topLeft() + QPoint(0, -2));
        doc.setDefaultFont(Statics::getOptions()->m_logFont);
        doc.drawContents(painter);
        painter->restore();
    }
}

// ------ LogViewTableModel -------

LogViewTableModel::LogViewTableModel(QObject* parent, const ExtendedFilterModel* filterModel)
    : QAbstractTableModel(parent),
      m_extendedFilterModel(filterModel)
{
    connect(this, &LogViewTableModel::rowsAboutToBeRemoved,
            this, &LogViewTableModel::slotRowsAboutToBeRemoved);
    startTimer(100);
}

int LogViewTableModel::rowCount(const QModelIndex&) const
{
    return m_logEntriesVisible.count();
}

int LogViewTableModel::columnCount(const QModelIndex&) const
{
    return 1;
}

QVariant LogViewTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0)
    {
        return QVariant();
    }
    if (role == Qt::DisplayRole)
    {
        const LogEntryPtr& ret = m_logEntriesVisible.at(index.row());
        return qVariantFromValue(ret);
    }
    else if (role == Qt::FontRole)
    {
        return Statics::getOptions()->m_logFont;
    }
    return QVariant();
}

void LogViewTableModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_logEntriesVisible.count());
    m_logEntriesVisible.clear();
    endRemoveRows();
    m_logEntries.clear();
    redrawVisibleRows();
}

void LogViewTableModel::erase(int count)
{
    if (count < m_logEntries.size())
    {
        int lastSerialToBeDeleted = m_logEntries.at(count - 1)->getSerial();
        for (int i = 0; i < m_logEntriesVisible.size(); i++)
        {
            if (lastSerialToBeDeleted == m_logEntriesVisible.at(i)->getSerial())
            {
                beginRemoveRows(QModelIndex(), 0, i);
                m_logEntriesVisible.remove(0, i + 1);

                endRemoveRows();
                break;
            }
        }
        m_logEntries.remove(0, count);
    }
    else
    {
        beginRemoveRows(QModelIndex(), 0, m_logEntriesVisible.size() - 1);
        m_logEntriesVisible.clear();
        endRemoveRows();
        m_logEntries.clear();
    }
}

void LogViewTableModel::slotRowsAboutToBeRemoved(const QModelIndex&, int first, int last)
{
    emit signalDeletingRows(last - first + 1);
}

bool LogViewTableModel::logEntryIsVisible(LogEntryPtr logEntry)
{
    return (logEntry->isInScope(m_logSeverity, m_textFilter) && m_extendedFilterModel->isMatched(logEntry)) ||
            logEntry->getText().startsWith(Statics::AntiLogMessage);
}

void LogViewTableModel::append(QVector<LogEntryPtr> logEntries)
{
    int toBeDeleted = m_logEntries.size() + logEntries.size() -
            Statics::getOptions()->m_maxRowsInLogModel;

    if (toBeDeleted > 0)
    {
        erase(toBeDeleted);
        // 1. Nothing is done yet with the new logEntries if they alone exceed the max rows limit
        // 2. When entries are deleted the effect is that the view scrolls,
        //    this needs fixing. The view should keep the displayed log entries fixed
    }

    QVector<LogEntryPtr> newVisibleLogEntries;

    foreach(auto logEntryPtr, logEntries)
    {
        if (logEntryIsVisible(logEntryPtr))
        {
            newVisibleLogEntries.append(logEntryPtr);
        }
    }

    m_logEntries.append(logEntries);

    if (!newVisibleLogEntries.empty())
    {
        int first = m_logEntriesVisible.count();
        int last = m_logEntriesVisible.count() + newVisibleLogEntries.size() - 1;
        beginInsertRows(QModelIndex{}, first, last);
        m_logEntriesVisible.append(newVisibleLogEntries);
        endInsertRows();
        emit newEntriesAdded();
    }
}

bool LogViewTableModel::removeRows(int row, int count, const QModelIndex&)
{
    QVector<int> serialsToDelete;

    for (int i = row; i < row + count; i++)
    {
        serialsToDelete.append(m_logEntriesVisible.at(i)->getSerial());
    }

    if (!serialsToDelete.isEmpty())
    {
        while (!serialsToDelete.isEmpty())
        {
            int nextSerial = serialsToDelete.takeAt(0);

            for (int i = 0; i < m_logEntries.size(); i++)
            {
                if (m_logEntries.at(i)->getSerial() == nextSerial)
                {
                    m_logEntries.remove(i);
                    break;
                }
            }
        }
        redrawVisibleRows();
        return true;
    }
    return false;
}

void LogViewTableModel::timerEvent(QTimerEvent*)
{
    if (!m_logEntriesPending.empty())
    {
        append(m_logEntriesPending);
        m_logEntriesPending.clear();
    }
}

void LogViewTableModel::append(LogEntryPtr logEntry)
{
    m_logEntriesPending.append(logEntry);
}

void LogViewTableModel::newLogSeverity(const QString& severity)
{
    m_logSeverity = severity;
    redrawVisibleRows();
}

void LogViewTableModel::setTextFilter(const QString& textFilter)
{
    m_textFilter = textFilter;
    redrawVisibleRows();
}

void LogViewTableModel::redrawVisibleRows()
{
    beginResetModel();
    m_logEntriesVisible.clear();
    endResetModel();

    // flush the incomming buffer to include any latecomers
    m_logEntries.append(m_logEntriesPending);
    m_logEntriesPending.clear();

    beginInsertRows(QModelIndex{}, m_logEntriesVisible.count(), m_logEntriesVisible.count());
    foreach (auto logEntry, m_logEntries)
    {
        if (logEntryIsVisible(logEntry))
        {
            logEntry->invalidateCachedHtml();
            m_logEntriesVisible.append(logEntry);
        }
    }
    endInsertRows();
}
