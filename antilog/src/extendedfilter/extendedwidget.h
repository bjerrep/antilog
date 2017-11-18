#pragma once

#include <QWidget>

class ExtendedFilterItem;

namespace Ui {
class ExtendedWidget;
}

class ExtendedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExtendedWidget(ExtendedFilterItem* filterItem, QWidget* parent = 0);
    ~ExtendedWidget();

    void setEnableState(Qt::CheckState enabled);
    void setSeverity(QString severity);

private slots:
    void slotCheckBoxEnableClicked(bool enabled);
    void slotComboBoxSeverityChanged(const QString& severity);

signals:
    void signalCheckBoxEnablePressed(bool enabled, ExtendedFilterItem* filterItem);
    void signalComboBoxSeverityChanged(const QString& severity, ExtendedFilterItem* filterItem);

public:
    Ui::ExtendedWidget* ui;
    ExtendedFilterItem* m_extendedFilterItem;
};

