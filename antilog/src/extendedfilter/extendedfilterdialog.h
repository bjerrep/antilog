#pragma once

#include <QDialog>

class ExtendedFilterItem;
class ExtendedFilterModel;
class ExtendedWidget;
class QStandardItemModel;

namespace Ui {
class ExtendedFilterDialog;
}

class ExtendedFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtendedFilterDialog(QWidget* parent, ExtendedFilterModel* filterModel);
    ~ExtendedFilterDialog();

    void add(ExtendedFilterItem* source, ExtendedFilterItem* module);

private:
    void drawFilterModel();
    ExtendedWidget* makeWidget(ExtendedFilterItem* filterItem);
    void setupTreeView();

private slots:
    void slotAddFilterItems(ExtendedFilterItem* source, ExtendedFilterItem* module);
    void expanded(const QModelIndex& index);
    void collapsed(const QModelIndex& index);
    void slotWidgetEnablePressed(bool enabled, ExtendedFilterItem* filterItem);
    void slotWidgetSeverityChanged(const QString& severity, ExtendedFilterItem* filterItem);
    void on_checkBoxEnabled_clicked(bool checked);
    void on_checkBoxAutoUpdate_clicked(bool checked);
    void on_pushButtonEnableAll_clicked();
    void on_pushButtonDisableAll_clicked();
    void on_pushButton_clicked();
    void on_checkBoxSorted_clicked(bool checked);
    void on_comboBoxSeverity_currentIndexChanged(const QString& arg1);
    void on_checkBoxPersist_clicked(bool checked);
    void on_pushButtonClear_clicked();

private:
    Ui::ExtendedFilterDialog* ui;
    ExtendedFilterModel* m_extendedFilterModel;
    QStandardItemModel* m_standardItemModel;
    QVector<std::array<ExtendedFilterItem*, 2>> m_buffered;
};

