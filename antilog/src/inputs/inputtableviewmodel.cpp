#include "inputtableviewmodel.h"
#include "sourcevisitors.h"
#include "inputlist.h"
#include "input.h"

InputTableViewModel::InputTableViewModel(InputList* inputList, QObject* parent)
    : QAbstractTableModel(parent),
      m_inputList(inputList)
{
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
        if (m_inputList->getInput(index.row())->isOn())
            return QColor(235,235,184 + 50 * (index.row() % 2));
        else
            return QColor(255,255,255);
    }
    return QVariant();
}

int InputTableViewModel::append(Input* sourceLine)
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
