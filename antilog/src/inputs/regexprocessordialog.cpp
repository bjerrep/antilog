#include "regexprocessordialog.h"
#include "ui_regexprocessordialog.h"
#include "inputprocessors.h"
#include "format/formatdialog.h"
#include "format/formatscheme.h"
#include "format/formatschememodel.h"
#include "antilog.h"



RegexProcessorDialog::RegexProcessorDialog(RegexProcessor* regexProcessor, AntiLog* parent) :
    QDialog(parent),
    ui(new Ui::RegexProcessorDialog),
    m_regexProcessor(regexProcessor),
    m_antiLog(parent)
{
    ui->setupUi(this);

    ui->iconLayout->addWidget(Statics::widgetIcon(Statics::RegexProcessorResource));
    ui->name->setText(regexProcessor->getName());

    ui->testRegexComboBox->addItem(regexProcessor->getRegex());
    ui->testRegexComboBox->addItem("spdlog: \\[(\\S+) (\\S+)\\] \\[(\\S+)\\] \\[(\\S+)\\] (.*)");
    ui->testRegexComboBox->addItem("antitestgui, source & modules: (\\S+) (?:\\S+) - (.*)");

    ui->checkBoxOnlyOutputMatches->setChecked(regexProcessor->m_onlyPassRegexMatches);

    QString scheme = regexProcessor->getScheme()->getName();
    ui->comboBoxFormat->addItems(parent->getFormatSchemeModel()->getSchemeNames());
    ui->comboBoxFormat->setCurrentIndex(parent->getFormatSchemeModel()->getSchemeNameIndex(scheme));
    ui->lineEditTestSource->setText("[2017-06-09 00:09:46.850] [console] [info] Support for floats 1.23");

    updateDialog();

    setModal(true);

    if (exec() == QDialog::Accepted)
    {
        regexProcessor->setName(ui->name->text());
        const QString scheme = ui->comboBoxFormat->currentText();
        regexProcessor->setSchemeFromName(scheme);
        regexProcessor->m_regex = ui->testRegexComboBox->currentText();
        regexProcessor->m_onlyPassRegexMatches = ui->checkBoxOnlyOutputMatches->isChecked();
    }
}

RegexProcessorDialog::~RegexProcessorDialog()
{
    delete ui;
}

void RegexProcessorDialog::updateDialog()
{
    ui->listWidgetActive->clear();
    foreach (auto cellFormat, m_regexProcessor->getScheme()->getColumnSetup().getColumnVector())
    {
        if (cellFormat->isEnabled())
        {
            QString name = m_antiLog->getGlobalColumnConfig()->getName(cellFormat->getCellType());
            ui->listWidgetActive->addItem(name);
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

void RegexProcessorDialog::layoutChanged(QListWidgetItem*, QListWidgetItem*)
{
    m_nofActiveCells = ui->listWidgetActive->count();
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

void RegexProcessorDialog::on_testRegexComboBox_editTextChanged(const QString &arg1)
{
    m_regexProcessor->setRegex(arg1);
    updateDialog();
}

void RegexProcessorDialog::on_testRegexComboBox_currentIndexChanged(int index)
{
    if (index > 0)
    {
        ui->testRegexComboBox->setItemText(0, ui->testRegexComboBox->itemText(index).split(": ")[1]);
        ui->testRegexComboBox->setCurrentIndex(0);
        updateDialog();
    }
}
