#include "formatwidget.h"
#include "ui_formatwidget.h"
#include "statics.h"
#include "loglevels.h"
#include "formatschememodel.h"
#include "tableformat.h"
#include "formatrule.h"
#include <QComboBox>
#include <QCheckBox>


FormatWidget::FormatWidget(FormatRule* formatEntry, const TableFormat& tableFormat, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::FormatWidget),
    m_formatRule(formatEntry)
{
    ui->setupUi(this);

    auto currentScope = formatEntry->m_scope;
    ui->comboBoxScope->clear();
    QStringList categories;
    categories << Statics::FormatColumnAny;
    categories << tableFormat.getEnabledColumns();
    ui->comboBoxScope->addItems(categories);
    ui->comboBoxScope->setCurrentText(currentScope);

    auto currentOperation = formatEntry->matchingRuleAsString();
    ui->comboBoxOperation->clear();
    ui->comboBoxOperation->addItems(FormatRule::getMatchingRules());
    ui->comboBoxOperation->setCurrentText(currentOperation);

    ui->comboBoxSearchTerm->setCurrentText(formatEntry->searchTerm());

    const QString& currentColor = formatEntry->colorAsString();
    ui->comboBoxColor->clear();
    ui->comboBoxColor->addItems(FormatRule::getColorList());
    ui->comboBoxColor->setCurrentText(currentColor);

    const QString& currentBgColor = formatEntry->bgColorAsString();
    ui->comboBoxBgcolor->clear();
    ui->comboBoxBgcolor->addItems(FormatRule::getColorList());
    ui->comboBoxBgcolor->setCurrentText(currentBgColor);

    const QString& currentRange = formatEntry->formatAreaAsString();
    ui->comboBoxSelection->clear();
    ui->comboBoxSelection->addItems(FormatRule::getFormatAreas(true));
    ui->comboBoxSelection->setCurrentText(currentRange);

    ui->checkBoxBold->setChecked(formatEntry->m_bold);
    ui->checkBoxCase->setChecked(formatEntry->m_case);

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

void FormatWidget::on_comboBoxScope_currentTextChanged(const QString &arg1)
{
    m_formatRule->m_scope = arg1;
    emit signalFormatRuleChanged();
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
