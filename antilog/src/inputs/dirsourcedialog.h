#pragma once

#include "inputsources.h"

#include <QDialog>

class AntiLog;

namespace Ui {
class DirSourceDialog;
}

class DirSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DirSourceDialog(DirSource& dirSource, AntiLog* parent);
    ~DirSourceDialog();

    void updateUi(const QString& dir);

private slots:
    void on_DirSelectPushButton_clicked();
    void on_OkPushButton_clicked();
    void on_CancelPushButton_clicked();
    void on_RecurseCheckBox_clicked(bool checked);
    void on_FileMaskLineEdit_editingFinished();
    void on_TargetLineEdit_returnPressed();

private:
    Ui::DirSourceDialog* ui;
};
