#include "formatdialog.h"
#include "ui_formatdialog.h"
#include "formatwidget.h"
#include "formatschememodel.h"
#include "formatrule.h"
#include "antilog.h"
#include "statics.h"
#include "inputlist.h"
#include "sourceandprocessor.h"

#include <QInputDialog>
#include <QDir>
#include <QMessageBox>
#include <QLineEdit>
#include <QBoxLayout>

FormatDialog::FormatDialog(const QString& schemeName, QWidget* parent, const InputList* inputList)
    : QDialog(parent),
      ui(new Ui::FormatDialog),
      m_inputList(inputList)
{
    ui->setupUi(this);

    redrawFormatScheme(schemeName);
}

FormatDialog::~FormatDialog()
{
    delete ui;
}

void FormatDialog::redrawFormatScheme(const QString& schemeName)
{
    if (schemeName != ui->comboBoxSchemeName->currentText())
    {
        ui->comboBoxSchemeName->clear();
        ui->comboBoxSchemeName->addItems(Statics::formatSchemeModel->getSchemeNames());
        ui->comboBoxSchemeName->setCurrentText(schemeName);
    }

    m_formatScheme = Statics::formatSchemeModel->getFormatScheme(schemeName);
    ui->listWidgetFormatEntries->clear();

    foreach (auto fs, m_formatScheme->getEntries())
    {
        auto formatWidget = new FormatWidget(fs, m_formatScheme->getTableFormat(), this);
        connect(formatWidget, &FormatWidget::signalDeleteWidget,
                this, &FormatDialog::slotDeleteWidget);
        connect(formatWidget, &FormatWidget::signalFormatRuleChanged,
                this, &FormatDialog::slotFormatRuleChanged);

        auto listWidgetItem = new QListWidgetItem();
        ui->listWidgetFormatEntries->addItem(listWidgetItem);
        ui->listWidgetFormatEntries->setItemWidget(listWidgetItem, formatWidget);
        listWidgetItem->setSizeHint(formatWidget->sizeHint());
    }

    ui->pushButtonAddRule->setEnabled(schemeName != Statics::NoneScheme);

    bool deleteEnabled = (schemeName != Statics::NoneScheme) && m_inputList;
    if (deleteEnabled && m_inputList->findFormatSchemeUsageCount(schemeName) > 0)
    {
        deleteEnabled = false;
    }
    ui->pushButtonDeleteScheme->setEnabled(deleteEnabled);

    connect(this, &FormatDialog::signalFormatRuleChanged,
            Statics::antiLog, &AntiLog::slotFormatRuleChanged);

    ui->active->clear();
    ui->inactive->clear();
    foreach (auto&& cellFormat, m_formatScheme->getTableFormat().getCells())
    {
        if (cellFormat.isEnabled())
        {
            ui->active->addItem(cellFormat.getName());
        }
        else
        {
            ui->inactive->addItem(cellFormat.getName());
        }
    }
    ui->active->setEnabled(schemeName != Statics::NoneScheme);
    ui->inactive->setEnabled(schemeName != Statics::NoneScheme);
}

void FormatDialog::slotDeleteWidget(FormatWidget* sender)
{
    m_formatScheme->deleteFormatEntry(sender->formatEntry());
    redrawFormatScheme(m_formatScheme->name());
}

void FormatDialog::slotFormatRuleChanged()
{
    emit signalFormatRuleChanged();
}

void FormatDialog::on_comboBoxSchemeName_currentTextChanged(const QString& arg1)
{
    redrawFormatScheme(arg1);
}

void FormatDialog::on_pushButtonClose_clicked()
{
    QStringList active;
    for (int i = 0; i < ui->active->model()->rowCount(); i++)
    {
        active << ui->active->item(i)->text();
    }
    m_formatScheme->m_tableFormat.setActiveCells(active);

    close();
    setResult(QDialog::Accepted);
}

void FormatDialog::on_pushButtonNewScheme_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "New format scheme",
                                         "Name", QLineEdit::Normal,
                                         "default", &ok);
    if (ok)
    {
        foreach (auto&& formatScheme, Statics::formatSchemeModel->getFormatSchemes())
        {
            if (formatScheme->name() == name)
            {
                QMessageBox::critical(this, "Unable to create format scheme",
                                      "Name already exist",
                                      QMessageBox::Ok);
                return;
            }
        }

        Statics::formatSchemeModel->addScheme(name);
        redrawFormatScheme(name);
    }
}

void FormatDialog::on_pushButtonDeleteScheme_clicked()
{
    QString name = ui->comboBoxSchemeName->currentText();
    Statics::formatSchemeModel->deleteScheme(name);
    redrawFormatScheme(Statics::NoneScheme);
}

void FormatDialog::on_pushButtonAddRule_clicked()
{
    auto formatRule = new FormatRule();
    m_formatScheme->add(formatRule);
    redrawFormatScheme(m_formatScheme->name());
}
