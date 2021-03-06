﻿#include "formatdialog.h"
#include "ui_formatdialog.h"
#include "formatwidget.h"
#include "formatscheme.h"
#include "formatschememodel.h"
#include "formatrule.h"
#include "inputs/inputlist.h"
#include "antilog.h"

#include <QInputDialog>
#include <QMessageBox>

FormatDialog::FormatDialog(const QString& schemeName, QWidget* parent, const InputList* inputList)
    : QDialog(parent),
      ui(new Ui::FormatDialog),
      m_inputList(inputList),
      m_formatSchemeModel(Statics::instAntiLog()->getFormatSchemeModel())
{
    ui->setupUi(this);

    setSchemeFromNameAndRedraw(schemeName);

    ui->inactive->setFormatScheme(m_formatScheme);
    connect(ui->active, &FormatDialogListWidget::signalDrag,
            this, &FormatDialog::updateFormatModel, Qt::QueuedConnection);
    connect(ui->inactive, &FormatDialogListWidget::signalDrag,
            this, &FormatDialog::updateFormatModel, Qt::QueuedConnection);
}

FormatDialog::~FormatDialog()
{
    delete ui;
}

void FormatDialog::setSchemeFromNameAndRedraw(const QString& schemeName)
{
    if (schemeName != ui->comboBoxSchemeName->currentText())
    {
        ui->comboBoxSchemeName->blockSignals(true);
        ui->comboBoxSchemeName->clear();
        ui->comboBoxSchemeName->addItems(m_formatSchemeModel->getSchemeNames());
        ui->comboBoxSchemeName->setCurrentText(schemeName);
        ui->comboBoxSchemeName->blockSignals(false);
    }

    m_formatScheme = m_formatSchemeModel->getFormatScheme(schemeName);
    redraw();
}

void FormatDialog::redraw()
{
    ui->listWidgetFormatEntries->clear();

    foreach (auto& formatRule, m_formatScheme->getEntries())
    {
        auto formatWidget = new FormatWidget(formatRule, m_formatScheme->getColumnSetup(), this);
        connect(formatWidget, &FormatWidget::signalDeleteWidget,
                this, &FormatDialog::slotDeleteWidget);
        connect(formatWidget, &FormatWidget::signalFormatRuleChanged,
                this, &FormatDialog::slotFormatRuleChanged);

        auto listWidgetItem = new QListWidgetItem();
        ui->listWidgetFormatEntries->addItem(listWidgetItem);
        ui->listWidgetFormatEntries->setItemWidget(listWidgetItem, formatWidget);
        listWidgetItem->setSizeHint(formatWidget->sizeHint());
    }

    auto formatSchemeName = m_formatScheme->getName();

    ui->active->clear();
    ui->inactive->clear();
    ui->inactive->setFormatScheme(m_formatScheme);

    foreach (const SchemeColumn* column, m_formatScheme->getColumnSetup().getColumnVector())
    {
        QString name = Statics::instColumnLibrary()->getName(column->getCellType());
        if (column->isEnabled())
        {
            ui->active->addItem(name);
        }
        else
        {
            ui->inactive->addItem(name);
        }
    }

    auto isUserScheme = formatSchemeName != Statics::NoneScheme;
    ui->pushButtonAddRule->setEnabled(isUserScheme);
    ui->active->setEnabled(isUserScheme);
    ui->inactive->setEnabled(isUserScheme);

    bool deleteEnabled = isUserScheme && m_inputList;
    if (deleteEnabled && m_inputList->findFormatSchemeUsageCount(formatSchemeName) > 0)
    {
        deleteEnabled = false;
    }
    ui->pushButtonDeleteScheme->setEnabled(deleteEnabled);
}

void FormatDialog::slotDeleteWidget(FormatWidget* sender)
{
    m_formatScheme->deleteFormatRule(sender->formatEntry());
    redraw();
}

void FormatDialog::slotFormatRuleChanged()
{
    emit signalFormatRuleChanged();
}

void FormatDialog::updateFormatModel()
{
    QStringList moduleIdList;
    for (int i = 0; i < ui->active->model()->rowCount(); i++)
    {
        moduleIdList << ui->active->item(i)->text();
    }
    m_formatScheme->getColumnSetup().setActiveCells(moduleIdList);
    redraw();
}

void FormatDialog::on_comboBoxSchemeName_currentTextChanged(const QString& arg1)
{
    updateFormatModel();
    setSchemeFromNameAndRedraw(arg1);
}

void FormatDialog::on_pushButtonClose_clicked()
{
    updateFormatModel();
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
        foreach (auto&& formatScheme, m_formatSchemeModel->getFormatSchemes())
        {
            if (formatScheme->getName() == name)
            {
                QMessageBox::critical(this, "Unable to create format scheme",
                                      "Name already exist",
                                      QMessageBox::Ok);
                return;
            }
        }

        m_formatSchemeModel->addScheme(name);
        setSchemeFromNameAndRedraw(name);
    }
}

void FormatDialog::on_pushButtonDeleteScheme_clicked()
{
    QString name = ui->comboBoxSchemeName->currentText();
    m_formatSchemeModel->deleteScheme(name);
    setSchemeFromNameAndRedraw(Statics::NoneScheme);
}

void FormatDialog::on_pushButtonAddRule_clicked()
{
    auto formatRule = new FormatRule();
    m_formatScheme->addFormatRule(formatRule);
    redraw();
}
