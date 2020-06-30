#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "column/columndialog.h"
#include "column/globalcolumnconfig.h"
#include "statics.h"
#include "options.h"
#include "antilog.h"

#include <QFontDialog>


OptionsDialog::OptionsDialog(Options& options, AntiLog* parent, GlobalColumnConfig* columnTableFormat)
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

void OptionsDialog::slotCurrentApplicationFontChanged(const QFont &font)
{
    m_options.setAppFont(font);
    emit signalAppFontChanged();
}

void OptionsDialog::slotCurrentLogViewFontChanged(const QFont &font)
{
    m_options.setLogFont(font);
    emit signalLogViewFontChanged();
}

void OptionsDialog::on_pushButtonApplicationFont_clicked()
{
    bool ok;
    QFontDialog fontDialog(this);

    // this doesn't work ? why not ?
    connect(&fontDialog, &QFontDialog::currentFontChanged,
            this, &OptionsDialog::slotCurrentApplicationFontChanged);

    QFont font = fontDialog.getFont(&ok, Statics::s_options->getAppFont(), this);
    if (ok)
    {
        m_options.setAppFont(font);
    }
    emit signalAppFontChanged();
}


void OptionsDialog::on_pushButtonLogViewFont_clicked()
{
    bool ok;
    QFontDialog fontDialog(this);

    connect(&fontDialog, &QFontDialog::currentFontChanged,
            this, &OptionsDialog::slotCurrentLogViewFontChanged);

    QFont font = fontDialog.getFont(&ok, Statics::s_options->getLogFont(), this);
    if (ok)
    {
        m_options.setLogFont(font);
    }
    emit signalLogViewFontChanged();
}
