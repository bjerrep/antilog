#include "extendedfilterdialog.h"
#include "ui_extendedfilterdialog.h"
#include "extendedfiltermodel.h"
#include "extendedwidget.h"
#include "ui_extendedwidget.h"
#include "statics.h"
#include "loglevels.h"

#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>


ExtendedFilterDialog::ExtendedFilterDialog(QWidget* parent, ExtendedFilterModel* filterModel)
    : QDialog(parent),
      ui(new Ui::ExtendedFilterDialog),
      m_filterModel(filterModel)
{
    ui->setupUi(this);
    m_stdItemModel = new QStandardItemModel(0, 2, this);
    m_stdItemModel->setHorizontalHeaderLabels({"Source & Modules", "Enable & Severity"});
    ui->treeView->setModel(m_stdItemModel);
    ui->treeView->setUniformRowHeights(true);
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeView->setColumnWidth(1, 130);

    ui->checkBoxEnabled->setChecked(filterModel->m_newSourcesEnabled);
    ui->checkBoxAutoUpdate->setChecked(true);
    ui->checkBoxSorted->setChecked(true);
    ui->checkBoxPersist->setChecked(filterModel->m_persistSourcesAndModules);

    QStringList severities = Statics::logLevels->getCategoryNames();
    ui->comboBoxSeverity->addItems(severities);
    ui->comboBoxSeverity->setCurrentText(filterModel->getDefaultSeverity());

    connect(ui->treeView, &QTreeView::expanded,
            this, &ExtendedFilterDialog::expanded);

    connect(ui->treeView, &QTreeView::collapsed,
            this, &ExtendedFilterDialog::collapsed);

    connect(filterModel, &ExtendedFilterModel::signalNewFilterItems,
            this, &ExtendedFilterDialog::slotAddFilterItems);

    drawFilterModel();
}

ExtendedFilterDialog::~ExtendedFilterDialog()
{
    delete ui;
}

ExtendedWidget* ExtendedFilterDialog::makeWidget(ExtendedFilterItem* filterItem)
{
    auto widget = new ExtendedWidget(filterItem);

    connect(widget, &ExtendedWidget::signalCheckBoxEnablePressed,
            this, &ExtendedFilterDialog::slotWidgetEnablePressed);
    connect(widget, &ExtendedWidget::signalComboBoxSeverityChanged,
            this, &ExtendedFilterDialog::slotWidgetSeverityChanged);

    return widget;
}

void ExtendedFilterDialog::drawFilterModel()
{
    for (int i = 0; i < m_filterModel->m_sources.count(); i++)
    {
        auto source = m_filterModel->m_sources[i];
        auto firstModule = source->getModules()[0];
        add(source, firstModule);

        for (int j = 1; j < source->getModules().count(); j++)
        {
            auto nextModule = m_filterModel->m_sources[i]->getModules()[j];
            add(source, nextModule);
        }
    }
}

void ExtendedFilterDialog::expanded(const QModelIndex& index)
{
    m_filterModel->sourceAt(index.row())->setViewExpanded(true);
}

void ExtendedFilterDialog::collapsed(const QModelIndex& index)
{
    m_filterModel->sourceAt(index.row())->setViewExpanded(false);
}

void ExtendedFilterDialog::slotWidgetEnablePressed(bool enabled, ExtendedFilterItem* filterItem)
{
    auto state = enabled ? Qt::Checked : Qt::Unchecked;
    filterItem->setEnableState(state);
    if (filterItem->isParent())
    {
        foreach (auto module, filterItem->getModules())
        {
            module->setEnableState(state);
        }
    }
    else
    {
        auto source = filterItem->getParent();
        for (int i = 0; i < source->getModules().count() - 1; ++i)
        {
            if (source->getModules().at(i)->getEnableState() != source->getModules().at(i + 1)->getEnableState())
            {
                source->setEnableState(Qt::PartiallyChecked);
                return;
            }
        }
        source->setEnableState(state);
    }
}

void ExtendedFilterDialog::slotWidgetSeverityChanged(const QString& severity, ExtendedFilterItem* filterItem)
{
    QObject::blockSignals(true);
    filterItem->setSeverity(severity);
    if (filterItem->isParent())
    {
        foreach (auto source, filterItem->getModules())
        {
            source->setSeverity(severity);
        }
    }
    else
    {
        auto parent = filterItem->getParent();
        auto modules = parent->getModules();
        for (int i = 0; i < modules.count() - 1; ++i)
        {
            if (modules.at(i)->getSeverity() != modules.at(i + 1)->getSeverity())
            {
                parent->setSeverity(Statics::logLevelFilterOverruled);
                return;
            }
        }
        parent->setSeverity(severity);
    }
}

void ExtendedFilterDialog::add(ExtendedFilterItem* source, ExtendedFilterItem* module)
{
    QStandardItem* stdItemSource = nullptr;
    auto sourceLookup = m_stdItemModel->findItems(source->getName());

    if (!sourceLookup.count())
    {
        // this is a new source entry
        int nextSourceIndex = m_stdItemModel->rowCount();
        stdItemSource = new QStandardItem(source->getName());
        m_stdItemModel->setItem(nextSourceIndex, 0, stdItemSource);

        auto sourceWidget = makeWidget(source);
        ui->treeView->setIndexWidget(m_stdItemModel->index(nextSourceIndex, 1), sourceWidget);
        if (source->isViewExpanded())
        {
            ui->treeView->expand(m_stdItemModel->index(nextSourceIndex, 0));
            if (ui->checkBoxSorted->isChecked())
            {
                stdItemSource->sortChildren(0);
            }
        }
    }
    else
    {
        stdItemSource = sourceLookup.at(0);
    }

    auto stdItemModule = new QStandardItem(module->getName());
    auto dummy = new QStandardItem();
    stdItemSource->appendRow({stdItemModule, dummy});

    auto moduleWidget = makeWidget(module);
    ui->treeView->setIndexWidget(dummy->index(), moduleWidget);
    if (ui->checkBoxSorted->isChecked())
    {
        stdItemSource->sortChildren(0);
    }
}

void ExtendedFilterDialog::slotAddFilterItems(ExtendedFilterItem* source, ExtendedFilterItem* module)
{
    if (!ui->checkBoxAutoUpdate->isChecked())
    {
        m_buffered.append({source, module});
        return;
    }
    if (!m_buffered.isEmpty())
    {
        foreach (auto filterSet, m_buffered)
        {
            add(filterSet[0], filterSet[1]);
        }
        m_buffered.clear();
    }
    add(source, module);
}

void ExtendedFilterDialog::on_checkBoxEnabled_clicked(bool checked)
{
    m_filterModel->m_newSourcesEnabled = checked ? Qt::Checked : Qt::Unchecked;
}

void ExtendedFilterDialog::on_checkBoxAutoUpdate_clicked(bool checked)
{
    if (checked)
    {
        foreach (auto filterSet, m_buffered)
        {
            add(filterSet[0], filterSet[1]);
        }
        m_buffered.clear();
    }
}

void ExtendedFilterDialog::on_pushButtonEnableAll_clicked()
{
    m_filterModel->enableAll(Qt::Checked);
}

void ExtendedFilterDialog::on_pushButtonDisableAll_clicked()
{
    m_filterModel->enableAll(Qt::Unchecked);
}

void ExtendedFilterDialog::on_pushButton_clicked()
{
    close();
    setResult(QDialog::Accepted);
}

void ExtendedFilterDialog::on_checkBoxSorted_clicked(bool checked)
{
    if (checked)
    {
        int nofSources = m_stdItemModel->rowCount();
        for (int i = 0; i < nofSources; i++)
        {
            auto index = m_stdItemModel->index(i, 0);
            auto item = m_stdItemModel->itemFromIndex(index);
            item->sortChildren(0);
        }
    }
}

void ExtendedFilterDialog::on_comboBoxSeverity_currentIndexChanged(const QString& arg1)
{
    m_filterModel->m_defaultSeverity = arg1;
}

void ExtendedFilterDialog::on_checkBoxPersist_clicked(bool checked)
{
    m_filterModel->m_persistSourcesAndModules = checked;
}

void ExtendedFilterDialog::on_pushButtonClear_clicked()
{
}
