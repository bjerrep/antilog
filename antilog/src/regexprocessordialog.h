#pragma once

#include <QDialog>
#include <QListWidgetItem>

class RegexProcessor;

namespace Ui {
class RegexProcessorDialog;
}

class RegexProcessorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegexProcessorDialog(RegexProcessor* regexProcessor, QWidget *parent);
    ~RegexProcessorDialog();

public slots:
    void updateDialog();
    void layoutChanged(QListWidgetItem *current, QListWidgetItem *previous);

public:
    Ui::RegexProcessorDialog *ui;

private slots:
    void on_lineEditRegex_textEdited(const QString &arg1);
    void on_lineEditTestSource_textEdited(const QString &arg1);
    void on_comboBoxFormat_currentTextChanged(const QString &arg1);
    void on_pushButtonEditFormatScheme_clicked();
    void on_pushButtonOk_clicked();
    void on_checkBoxOnlyOutputMatches_toggled(bool checked);

private:
    RegexProcessor* m_regexProcessor = nullptr;
    int m_nofActiveCells = 0;
};

