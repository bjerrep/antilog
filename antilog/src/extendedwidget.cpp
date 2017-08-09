#include "extendedwidget.h"
#include "ui_extendedwidget.h"
#include "extendedfiltermodel.h"
#include "statics.h"
#include "loglevels.h"

ExtendedWidget::ExtendedWidget(ExtendedFilterItem* filterItem, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ExtendedWidget),
    m_filterItem(filterItem)
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
    QStringList severities = Statics::s_logLevels->getCategoryNames();
    if (m_filterItem->isParent() && severity == Statics::logLevelFilterOverruled)
    {
        severities.insert(0, Statics::logLevelFilterOverruled);
    }
    ui->severity->clear();
    ui->severity->addItems(severities);
    ui->severity->setCurrentText(severity);
    ui->severity->blockSignals(false);
}

void ExtendedWidget::slotCheckBoxEnableClicked(bool enabled)
{
    setEnableState(enabled ? Qt::Checked : Qt::Unchecked);
    emit signalCheckBoxEnablePressed(enabled, m_filterItem);
}

void ExtendedWidget::slotComboBoxSeverityChanged(const QString& severity)
{
    emit signalComboBoxSeverityChanged(severity, m_filterItem);
}
