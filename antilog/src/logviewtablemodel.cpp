#include "logviewtablemodel.h"
#include "logentryformatter.h"
#include "logentry.h"
#include "options.h"

#include <QTextDocument>
#include <QStyleOptionViewItem>
#include <QApplication>

// ------ LogViewDelegate -------

LogViewDelegate::LogViewDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void LogViewDelegate::paint(QPainter *painter,
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
        doc.setDefaultFont(Statics::options->m_logFont);
        doc.drawContents(painter, QRect(QPoint(0, 0), option.rect.size()));
        painter->restore();
    }
}

QSize LogViewDelegate::sizeHint(const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

// ------ LogViewTableModel -------

LogViewTableModel::LogViewTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    startTimer(100);
}

QVariant LogViewTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        return m_visibleRows.at(section)->getSerial();
    }
    else if (role == Qt::FontRole)
    {
        return Statics::options->m_logFont;
    }

    return QVariant();
}

int LogViewTableModel::rowCount(const QModelIndex&) const
{
    return m_visibleRows.count();
}

int LogViewTableModel::columnCount(const QModelIndex&) const
{
    return 1;
}

QVariant LogViewTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const LogEntryPtr& ret = m_visibleRows.at(index.row());
        return qVariantFromValue(ret);
    }
    else if (role == Qt::FontRole)
    {
        return Statics::options->m_logFont;
    }
    return QVariant();
}

void LogViewTableModel::erase(int count)
{
    if (count < m_rowsTotal.size())
    {
        int firstSerialToBeDeleted = m_rowsTotal.at(count - 1)->getSerial();
        for (int i = 0; i < m_visibleRows.size(); i++)
        {
            if (firstSerialToBeDeleted == m_visibleRows.at(i)->getSerial())
            {
                beginRemoveRows(QModelIndex(), 0, i);
                m_visibleRows.remove(0, i + 1);
                endRemoveRows();
                break;
            }
        }
        m_rowsTotal.remove(0, count);
    }
    else
    {
        beginRemoveRows(QModelIndex(), 0, m_visibleRows.size() - 1);
        m_visibleRows.clear();
        endRemoveRows();
        m_rowsTotal.clear();
    }
}

void LogViewTableModel::append(QVector<LogEntryPtr> logEntries)
{
    int toBeDeleted = m_rowsTotal.size() + logEntries.size() -
                      Statics::options->m_maxRowsInLogModel;

    if (toBeDeleted > 0)
    {
        erase(toBeDeleted);
        // 1. Nothing is done yet with the new logEntries if they alone exceed the max rows limit
        // 2. When entries are deleted the effect is that the view scrolls,
        //    this needs fixing. The view should keep the displayed log entries fixed
    }

    QVector<LogEntryPtr> inScope;

    foreach(auto logEntryPtr, logEntries)
    {
        if (logEntryPtr->isInScope(m_logLevel, m_textFilter))
        {
            inScope.append(logEntryPtr);
        }
    }

    m_rowsTotal.append(logEntries);

    if (!inScope.empty())
    {
        int first = m_visibleRows.count();
        int last = m_visibleRows.count() + inScope.size() - 1;
        beginInsertRows(QModelIndex{}, first, last);
        m_visibleRows.append(inScope);
        endInsertRows();
        emit newEntriesAdded();
    }
}

void LogViewTableModel::timerEvent(QTimerEvent*)
{
    if(!m_rowsBuffered.empty())
    {
        append(m_rowsBuffered);
        m_rowsBuffered.clear();
    }
}

void LogViewTableModel::append(LogEntryPtr logEntry)
{
    m_rowsBuffered.append(logEntry);
}

void LogViewTableModel::newLogLevel(const QString& level)
{
    m_logLevel = level;
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
    m_visibleRows.clear();
    endResetModel();

    beginInsertRows(QModelIndex{}, m_visibleRows.count(), m_visibleRows.count());
    foreach (auto entry, m_rowsTotal)
    {
        if (entry->isInScope(m_logLevel, m_textFilter))
        {
            entry->invalidateCachedHtml();
            m_visibleRows.append(entry);
        }
    }
    endInsertRows();
}
