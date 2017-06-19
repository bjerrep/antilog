#pragma once

#include "options.h"

#include <QDialog>

namespace Ui
{
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(Options& options, QWidget *parent);
    ~OptionsDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::OptionsDialog *ui;
    Options& m_options;
};
