#pragma once

#include <QDialog>

class PassProcessor;

namespace Ui
{
class PassProcessorDialog;
}

class PassProcessorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PassProcessorDialog(PassProcessor* passProcessor, QWidget* parent);
    ~PassProcessorDialog();

    const PassProcessor* processor() const;

private slots:
    void on_comboBoxFormat_currentTextChanged(const QString &arg1);
    void on_name_textChanged(const QString &arg1);
    void on_pushButtonOk_clicked();
    void on_pushButton_clicked();

private:
    Ui::PassProcessorDialog* ui;
    PassProcessor* m_passProcessor;
};
