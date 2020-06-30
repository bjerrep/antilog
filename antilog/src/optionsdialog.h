#pragma once

#include "options.h"

#include <QDialog>

class GlobalColumnConfig;
class AntiLog;

namespace Ui
{
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    OptionsDialog(Options& options, AntiLog* parent, GlobalColumnConfig* columnTableFormat);
    ~OptionsDialog();

signals:
    void signalAppFontChanged();
    void signalLogViewFontChanged();

private slots:
    void on_pushButton_clicked();
    void on_editColumns_clicked();
    void on_pushButtonApplicationFont_clicked();
    void on_pushButtonLogViewFont_clicked();
    void slotCurrentApplicationFontChanged(const QFont &font);
    void slotCurrentLogViewFontChanged(const QFont &font);

private:
    Ui::OptionsDialog* ui;
    Options& m_options;
    GlobalColumnConfig* m_columnTableFormat = 0;
};
