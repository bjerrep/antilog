#include "formatdialoglistwidget.h"
#include "formatscheme.h"

#include <QDragMoveEvent>
#include <QStandardItemModel>

FormatDialogListWidget::FormatDialogListWidget(QWidget* parent) :
    QListWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDefaultDropAction(Qt::MoveAction);

    connect(this, &FormatDialogListWidget::itemChanged,
            this, &FormatDialogListWidget::slotContentChanged, Qt::QueuedConnection);
}

void FormatDialogListWidget::setFormatScheme(FormatScheme* formatScheme)
{
    m_formatScheme = formatScheme;
}

void FormatDialogListWidget::dragMoveEvent(QDragMoveEvent* event)
{
    bool isActiveListWidget = !m_formatScheme;

    if (isActiveListWidget)
    {
        event->accept();
    }
    else
    {
        auto model = QSharedPointer<QStandardItemModel>(new QStandardItemModel());
        model->dropMimeData(event->mimeData(), Qt::CopyAction, 0,0, QModelIndex());
        auto moduleid = model->item(0)->text();

        if (m_formatScheme->hasEntry(moduleid))
        {
            event->ignore();
        }
        else
        {
            event->accept();
        }
    }
}

void FormatDialogListWidget::slotContentChanged(QListWidgetItem* /*item*/)
{
    emit signalDrag();
}
