#include "columnwidget.h"
#include "ui_columnwidget.h"
#include "globalcolumn.h"


ColumnWidget::ColumnWidget(const QStringList& columnTypes, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ColumnWidget)
{
    ui->setupUi(this);
    blockSignals(true);
    ui->lineEdit_name->setText("New");
    ui->comboBox_type->addItems(columnTypes);
    ui->checkBox_enable->setChecked(false);
    blockSignals(false);
}

ColumnWidget::ColumnWidget(const GlobalColumn* columnFormat,
                           const QStringList& columnTypes,
                           QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ColumnWidget)
{
    ui->setupUi(this);
    blockSignals(true);
    ui->lineEdit_name->setText(columnFormat->getName());
    ui->comboBox_type->addItems(columnTypes);
    ui->comboBox_type->setCurrentText(columnFormat->getTypeAsString());
    ui->checkBox_enable->setChecked(columnFormat->isEnabled());
    columnFormat->save(m_json);
    blockSignals(false);
}

ColumnWidget::~ColumnWidget()
{
    delete ui;
}

int ColumnWidget::getHeight() const
{
    return ui->lineEdit_name->height();
}

GlobalColumn *ColumnWidget::getColumn() const
{
    if (m_deleted)
    {
        return nullptr;
    };
    auto columnFormat = new GlobalColumn(m_json);
    columnFormat->setName(ui->lineEdit_name->text());
    columnFormat->setTypeFromString(ui->comboBox_type->currentText());
    return columnFormat;
}

void ColumnWidget::on_pushButton_delete_clicked()
{
    m_deleted = true;
    emit signalColumnWidgetDeleted();
}
