#pragma once

#include <QStyledItemDelegate>


class RowHeightDelegate : public QStyledItemDelegate
{
public:
    RowHeightDelegate(int height);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int m_height;
};
