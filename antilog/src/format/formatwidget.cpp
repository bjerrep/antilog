#include "formatwidget.h"
#include "ui_formatwidget.h"
#include "statics.h"
#include "logentry/logseverities.h"
#include "formatschememodel.h"
#include "column/globalcolumnconfig.h"
#include "formatrule.h"
#include "column/schemecolumnmodel.h"

#include <QComboBox>
#include <QCheckBox>


FormatWidget::FormatWidget(FormatRule* formatRule, const SchemeColumnModel& columnSetup, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::FormatWidget),
    m_formatRule(formatRule)
{
    ui->setupUi(this);

    auto currentScope = GlobalColumn::staticGetTypeAsString(formatRule->m_columnType);
    ui->comboBoxSeverity->clear();
    QStringList categories;
    categories << GlobalColumn::staticGetTypeAsString(GlobalColumn::ANY);
    categories << columnSetup.getColumnTypeStringlist();
    ui->comboBoxSeverity->addItems(categories);
    ui->comboBoxSeverity->setCurrentText(currentScope);

    auto currentOperation = formatRule->matchingRuleAsString();
    ui->comboBoxOperation->clear();
    ui->comboBoxOperation->addItems(FormatRule::getMatchingRules());
    ui->comboBoxOperation->setCurrentText(currentOperation);

    ui->comboBoxSearchTerm->setCurrentText(formatRule->getSearchTerm());

    const QString& currentColor = formatRule->colorAsString();
    ui->comboBoxColor->clear();
    ui->comboBoxColor->addItems(FormatRule::getColorList());
    ui->comboBoxColor->setCurrentText(currentColor);

    const QString& currentBgColor = formatRule->bgColorAsString();
    ui->comboBoxBgcolor->clear();
    ui->comboBoxBgcolor->addItems(FormatRule::getColorList());
    ui->comboBoxBgcolor->setCurrentText(currentBgColor);

    const QString& currentRange = formatRule->formatAreaAsString();
    ui->comboBoxSelection->clear();
    ui->comboBoxSelection->addItems(FormatRule::getFormatAreas(true));
    ui->comboBoxSelection->setCurrentText(currentRange);

    ui->checkBoxBold->setChecked(formatRule->m_bold);
    ui->checkBoxCase->setChecked(formatRule->m_case);

    emit signalFormatRuleChanged();
}

FormatWidget::~FormatWidget()
{
    delete ui;
}

FormatRule* FormatWidget::formatEntry()
{
    return m_formatRule;
}

void FormatWidget::on_comboBoxOperation_currentIndexChanged(const QString &arg1)
{
    m_formatRule->setMatchingRuleFromString(arg1);
    emit signalFormatRuleChanged();
}

void FormatWidget::on_comboBoxSearchTerm_editTextChanged(const QString &arg1)
{
    m_formatRule->m_searchTerm = arg1;
    emit signalFormatRuleChanged();
}

void FormatWidget::on_checkBoxBold_clicked(bool checked)
{
    m_formatRule->m_bold = checked;
    emit signalFormatRuleChanged();
}

void FormatWidget::on_pushButtonDelete_clicked()
{
    emit signalDeleteWidget(this);
    emit signalFormatRuleChanged();
}

void FormatWidget::on_comboBoxColor_currentTextChanged(const QString &arg1)
{
    m_formatRule->setColorFromString(arg1);
    emit signalFormatRuleChanged();
}

void FormatWidget::on_comboBoxBgcolor_currentIndexChanged(const QString &arg1)
{
    m_formatRule->setBgColorFromString(arg1);
    emit signalFormatRuleChanged();
}

void FormatWidget::on_comboBoxSelection_currentTextChanged(const QString &arg1)
{
    m_formatRule->setFormatAreaFromString(arg1);
    emit signalFormatRuleChanged();
}

void FormatWidget::on_checkBoxCase_clicked(bool checked)
{
    m_formatRule->m_case = checked;
    emit signalFormatRuleChanged();
}

void FormatWidget::on_comboBoxSeverity_currentTextChanged(const QString &arg1)
{
    m_formatRule->m_columnType = GlobalColumn::staticGetTypeFromString(arg1);
    emit signalFormatRuleChanged();
}
