#include "columnwidget.h"
#include "ui_columnwidget.h"
#include "columndefinitions.h"

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

ColumnWidget::ColumnWidget(const Column* column,
                           const QStringList& columnTypes,
                           QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ColumnWidget)
{
    ui->setupUi(this);
    blockSignals(true);
    ui->lineEdit_name->setText(column->getName());
    ui->comboBox_type->addItems(columnTypes);
    ui->comboBox_type->setCurrentText(column->getCellTypeAsString());
    ui->checkBox_enable->setChecked(column->isEnabled());
    m_uid = column->getUid();
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

Column* ColumnWidget::getColumn() const
{
    auto column = new Column(ui->comboBox_type->currentText(),
                      ui->lineEdit_name->text(),
                      ui->checkBox_enable->checkState() == Qt::Checked,
                      m_uid);
    if (m_deleted)
    {
        column->setDeleted();
    };
    return column;
}

void ColumnWidget::on_pushButton_delete_clicked()
{
    m_deleted = true;
    emit signalColumnWidgetDeleted();
}
