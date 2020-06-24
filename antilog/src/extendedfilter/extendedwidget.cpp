#include "extendedwidget.h"
#include "ui_extendedwidget.h"
#include "extendedfiltermodel.h"
#include "statics.h"
#include "logentry/logseverities.h"

ExtendedWidget::ExtendedWidget(ExtendedFilterItem* filterItem, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ExtendedWidget),
    m_extendedFilterItem(filterItem)
{
    ui->setupUi(this);

    filterItem->setWidget(this);
    if (filterItem->isParent())
    {
        ui->enabled->setTristate();
    }
    setEnableState(filterItem->getEnableState());
    setSeverity(filterItem->getSeverity());

    connect(ui->enabled, &QCheckBox::clicked,
            this, &ExtendedWidget::slotCheckBoxEnableClicked);

    connect(ui->severity, &QComboBox::currentTextChanged,
            this, &ExtendedWidget::slotComboBoxSeverityChanged);
}

ExtendedWidget::~ExtendedWidget()
{
    delete ui;
}

void ExtendedWidget::setEnableState(Qt::CheckState state)
{
    ui->enabled->setCheckState(state);
}

void ExtendedWidget::setSeverity(QString severity)
{
    ui->severity->blockSignals(true);
    QStringList severities = Statics::s_logSeverities->getCategoryNames();
    if (m_extendedFilterItem->isParent() && severity == Statics::LogSeverityFilterOverruled)
    {
        severities.insert(0, Statics::LogSeverityFilterOverruled);
    }
    ui->severity->clear();
    ui->severity->addItems(severities);
    ui->severity->setCurrentText(severity);
    ui->severity->blockSignals(false);
}

void ExtendedWidget::slotCheckBoxEnableClicked(bool enabled)
{
    setEnableState(enabled ? Qt::Checked : Qt::Unchecked);
    emit signalCheckBoxEnablePressed(enabled, m_extendedFilterItem);
}

void ExtendedWidget::slotComboBoxSeverityChanged(const QString& severity)
{
    emit signalComboBoxSeverityChanged(severity, m_extendedFilterItem);
}
