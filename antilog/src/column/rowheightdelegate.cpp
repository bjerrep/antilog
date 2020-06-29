#include "rowheightdelegate.h"

RowHeightDelegate::RowHeightDelegate(int height) : QStyledItemDelegate()
{
    m_height = height;
}

QSize RowHeightDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    return QSize(0, m_height);
}
