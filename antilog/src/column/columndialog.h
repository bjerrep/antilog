#pragma once

#include <QDialog>

class ColumnWidget;
class GlobalColumnConfig;

namespace Ui {
class ColumnDialog;
}

class ColumnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColumnDialog(GlobalColumnConfig* columnTableFormat, QWidget *parent = 0);
    ~ColumnDialog();

private:
    void addWidget(ColumnWidget* columnWidget);
    void updateView();
    void updateColumnDefinitions();

private slots:
    void slotColumnWidgetDeleted();
    void on_pushButton_ok_clicked();
    void on_pushButton_new_clicked();
    void on_pushButton_reset_clicked();

private:
    Ui::ColumnDialog *ui;
    GlobalColumnConfig* m_globalColumnConfig;
};
