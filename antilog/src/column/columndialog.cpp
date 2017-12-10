#include "columndialog.h"
#include "ui_columndialog.h"
#include "columnwidget.h"
#include "schemecolumn.h"

ColumnDialog::ColumnDialog(GlobalColumnConfig* columnTableFormat, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ColumnDialog),
      m_globalColumnConfig(columnTableFormat)
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
    const auto& formatMap = m_globalColumnConfig->getTableCellFormatMap();
    for (auto it = formatMap.begin(); it != formatMap.end(); it++)
    {
        if (it.key() == GlobalColumn::ANY)
        {
            continue;
        }
        auto widget = new ColumnWidget(it.value(),
                                       m_globalColumnConfig->getColumnTypesAsStringlist(),
                                       ui->listWidget_columns);
        addWidget(widget);
    }
}

void ColumnDialog::updateColumnDefinitions()
{
    GlobalColumnList newColumnList;

    // add back existing ColumnWidgets (who haven't got the "deleted" flag set)
    for (int i = 0; i < ui->listWidget_columns->count(); i++)
    {
        auto item = ui->listWidget_columns->item(i);
        auto widget = qobject_cast<ColumnWidget*>(ui->listWidget_columns->itemWidget(item));
        auto columnFormat = widget->getColumn();
        if (columnFormat)
        {
        newColumnList.append(columnFormat);
        }
    }

    m_globalColumnConfig->refreshColumns(newColumnList);

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
    auto widget = new ColumnWidget(m_globalColumnConfig->getColumnTypesAsStringlist(),
                                   ui->listWidget_columns);
    addWidget(widget);
}

void ColumnDialog::on_pushButton_reset_clicked()
{
    m_globalColumnConfig->constructDefaultSetup();
    updateView();
}
