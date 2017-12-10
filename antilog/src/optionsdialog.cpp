#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "columndialog.h"
#include "globalcolumnconfig.h"
#include "statics.h"
#include "options.h"

OptionsDialog::OptionsDialog(Options& options, QWidget* parent, GlobalColumnConfig* columnTableFormat)
    : QDialog(parent),
    ui(new Ui::OptionsDialog),
    m_options(options),
    m_columnTableFormat(columnTableFormat)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

    ui->bytesToTail->setText(QString::number(options.m_numberOfBytesToTail));
    ui->maxTableLinesLineEdit->setText(QString::number(options.m_maxRowsInLogModel));
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::on_pushButton_clicked()
{
    m_options.m_numberOfBytesToTail = ui->bytesToTail->text().toInt();
    m_options.m_maxRowsInLogModel = ui->maxTableLinesLineEdit->text().toInt();
    close();
    setResult(QDialog::Accepted);
}

void OptionsDialog::on_editColumns_clicked()
{
    ColumnDialog dia(m_columnTableFormat);
    dia.exec();
}
