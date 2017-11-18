#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "columndialog.h"
#include "statics.h"
#include "options.h"

OptionsDialog::OptionsDialog(Options& options, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog),
    m_options(options)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

    ui->linesToPreloadLineEdit->setText(QString::number(options.m_numberOfLinesToLoad));
    ui->maxTableLinesLineEdit->setText(QString::number(options.m_maxRowsInLogModel));
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::on_pushButton_clicked()
{
    m_options.m_numberOfLinesToLoad = ui->linesToPreloadLineEdit->text().toInt();
    m_options.m_maxRowsInLogModel = ui->maxTableLinesLineEdit->text().toInt();
    close();
    setResult(QDialog::Accepted);
}

void OptionsDialog::on_editColumns_clicked()
{
    ColumnDialog dia(Statics::getOptions()->getColumnDefinitions());
    dia.exec();
}
