#pragma once

#include <QAbstractTableModel>

class InputList;
class SourceAndProcessor;

class InputTableViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    InputTableViewModel(InputList* inputList, QObject* parent);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int append(SourceAndProcessor* sourceLine);
    void removeRow(int row);

private:
    InputList* m_inputList;
};

