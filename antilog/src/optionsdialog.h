#pragma once

#include "options.h"

#include <QDialog>

class GlobalColumnConfig;

namespace Ui
{
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    OptionsDialog(Options& options, QWidget* parent, GlobalColumnConfig* columnTableFormat);
    ~OptionsDialog();

private slots:
    void on_pushButton_clicked();

    void on_editColumns_clicked();

private:
    Ui::OptionsDialog* ui;
    Options& m_options;
    GlobalColumnConfig* m_columnTableFormat = 0;
};
