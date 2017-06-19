#include "regexprocessordialog.h"
#include "ui_regexprocessordialog.h"
#include "statics.h"
#include "inputprocessors.h"
#include "tableformat.h"
#include "formatdialog.h"
#include "formatschememodel.h"
#include "logentry.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QStringList>
#include <QString>
#include <QTextBrowser>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>


RegexProcessorDialog::RegexProcessorDialog(RegexProcessor* regexProcessor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegexProcessorDialog),
    m_regexProcessor(regexProcessor)
{
    ui->setupUi(this);

    ui->iconLayout->addWidget(Statics::widgetIcon(Statics::RegexProcessorResource));
    ui->name->setText(regexProcessor->getName());
    ui->lineEditRegex->setText(regexProcessor->getRegex());

    QString scheme = regexProcessor->getScheme()->name();
    ui->comboBoxFormat->addItems(Statics::formatSchemeModel->getSchemeNames());
    ui->comboBoxFormat->setCurrentIndex(Statics::formatSchemeModel->getSchemeNameIndex(scheme));
    ui->lineEditTestSource->setText("[2017-06-09 00:09:46.850] [console] [info] Support for floats 1.23");

    updateDialog();

    setModal(true);

    if (exec() == QDialog::Accepted)
    {
        regexProcessor->setName(ui->name->text());
        const QString scheme = ui->comboBoxFormat->currentText();
        regexProcessor->setSchemeFromName(scheme);
        regexProcessor->m_regex = ui->lineEditRegex->text();
    }
}

RegexProcessorDialog::~RegexProcessorDialog()
{
    delete ui;
}

void RegexProcessorDialog::updateDialog()
{
    ui->listWidgetActive->clear();
    foreach (auto cellFormat, m_regexProcessor->getScheme()->getTableFormat().getCells())
    {
        if (cellFormat.isEnabled())
        {
            ui->listWidgetActive->addItem(cellFormat.getName());
        }
    }
    m_nofActiveCells = ui->listWidgetActive->count();

    const QStringList& res = m_regexProcessor->applyRegex(ui->lineEditTestSource->text());
    if (res.size() == m_nofActiveCells && !res.empty())
    {
        LogEntry logEntry(res, QString(), m_regexProcessor->getScheme());
        auto html = logEntry.getHtml();
        ui->textBrowserMatches->setHtml(html);
    }
    else
    {
        ui->textBrowserMatches->setText(
            QStringLiteral("error, found %1 groups but expected %2 groups").
            arg(res.size()).arg(m_nofActiveCells));
    }
}

void RegexProcessorDialog::layoutChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    m_nofActiveCells = ui->listWidgetActive->count();
    updateDialog();
}

void RegexProcessorDialog::on_lineEditRegex_textEdited(const QString &arg1)
{
    m_regexProcessor->setRegex(arg1);
    updateDialog();
}

void RegexProcessorDialog::on_lineEditTestSource_textEdited(const QString &arg1)
{
    ui->lineEditTestSource->setText(arg1);
    updateDialog();
}

void RegexProcessorDialog::on_comboBoxFormat_currentTextChanged(const QString &arg1)
{
    m_regexProcessor->setSchemeFromName(arg1);
    ui->pushButtonEditFormatScheme->setEnabled(arg1 != Statics::NoneScheme);
    updateDialog();
}

void RegexProcessorDialog::on_pushButtonEditFormatScheme_clicked()
{
    auto formatDialog = new FormatDialog(ui->comboBoxFormat->currentText(), this);
    formatDialog->exec();
    updateDialog();
}

void RegexProcessorDialog::on_pushButtonOk_clicked()
{
    close();
    setResult(QDialog::Accepted);
}

void RegexProcessorDialog::on_checkBoxOnlyOutputMatches_toggled(bool checked)
{
    m_regexProcessor->m_onlyPassRegexMatches = checked;
}
