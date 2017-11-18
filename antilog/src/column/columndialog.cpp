#include "columndialog.h"
#include "ui_columndialog.h"
#include "columnwidget.h"
#include "columndefinitions.h"

ColumnDialog::ColumnDialog(ColumnDefinitions *columnDefinitions, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ColumnDialog),
      m_columnDefinitions(columnDefinitions)
{
    ui->setupUi(this);

    updateView();
}

ColumnDialog::~ColumnDialog()
{
    delete ui;
}

void ColumnDialog::addWidget(ColumnWidget *columnWidget)
{
    auto item = new QListWidgetItem(ui->listWidget_columns);
    item->setSizeHint(QSize(item->sizeHint().width(), columnWidget->getHeight()));
    ui->listWidget_columns->addItem(item);
    ui->listWidget_columns->setItemWidget(item, columnWidget);
    connect(columnWidget, &ColumnWidget::signalColumnWidgetDeleted,
            this, &ColumnDialog::slotColumnWidgetDeleted, Qt::QueuedConnection);
}

void ColumnDialog::updateView()
{
    ui->listWidget_columns->clear();
    foreach (auto column, m_columnDefinitions->getColumnTypeList())
    {
        if (column->getCellType() == Column::ANY)
        {
            continue;
        }
        auto widget = new ColumnWidget(column,
                                       m_columnDefinitions->getColumnTypeStringlist(),
                                       ui->listWidget_columns);
        addWidget(widget);
    }
}

void ColumnDialog::updateColumnDefinitions()
{
    ColumnList columnList;

    // add back existing ColumnWidgets (who haven't got the "deleted" flag set)
    for (int i = 0; i < ui->listWidget_columns->count(); i++)
    {
        auto item = ui->listWidget_columns->item(i);
        auto widget = qobject_cast<ColumnWidget*>(ui->listWidget_columns->itemWidget(item));
        auto column = widget->getColumn();
        columnList.append(column);
    }

    m_columnDefinitions->refreshColumns(columnList);

    // and update the listview with the refreshed list
    updateView();
}

void ColumnDialog::slotColumnWidgetDeleted()
{
    updateColumnDefinitions();
}

void ColumnDialog::on_pushButton_ok_clicked()
{
    updateColumnDefinitions();
    close();
    setResult(QDialog::Accepted);
}

void ColumnDialog::on_pushButton_new_clicked()
{
    auto widget = new ColumnWidget(m_columnDefinitions->getColumnTypeStringlist(), ui->listWidget_columns);
    addWidget(widget);
}

void ColumnDialog::on_pushButton_reset_clicked()
{
    m_columnDefinitions->reset();
    updateView();
}
