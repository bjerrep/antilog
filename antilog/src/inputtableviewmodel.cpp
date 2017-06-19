#include "inputtableviewmodel.h"
#include "sourcevisitors.h"
#include "inputlist.h"
#include "sourceandprocessor.h"

InputTableViewModel::InputTableViewModel(InputList* inputList, QObject *parent)
    : QAbstractTableModel(parent),
      m_inputList(inputList)
{
}

QVariant InputTableViewModel::headerData(int section, Qt::Orientation /*orientation*/, int role) const
{
    if (role == Qt::DisplayRole && section < 4)
    {
        const static char* headers[] = {"Source", "", "Processor", "State"};
        return headers[section];
    }

    return QVariant();
}

int InputTableViewModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_inputList->count();
}

int InputTableViewModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 4;
}

QVariant InputTableViewModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        // all cells are widgets which are explicitly redrawn
    }
    if (role == Qt::BackgroundRole)
    {
        if (m_inputList->getSourceAndProcessor(index.row())->isOn())
            return QColor(235,235,184 + 50 * (index.row() % 2));
        else
            return QColor(255,255,255);
    }
    return QVariant();
}

int InputTableViewModel::append(SourceAndProcessor* sourceLine)
{
    beginInsertRows(QModelIndex{}, m_inputList->count(), m_inputList->count());
    m_inputList->appendAndConnect(sourceLine);
    endInsertRows();
    return m_inputList->count() - 1;
}

void InputTableViewModel::removeRow(int row)
{
    beginRemoveRows(QModelIndex{}, row, row);
    m_inputList->removeAt(row);
    endRemoveRows();
}
