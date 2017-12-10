#pragma once

#include <QDialog>
#include <QListWidgetItem>

class RegexProcessor;
class AntiLog;

namespace Ui {
class RegexProcessorDialog;
}

class RegexProcessorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegexProcessorDialog(RegexProcessor* regexProcessor, AntiLog* parent);
    ~RegexProcessorDialog();

public slots:
    void updateDialog();
    void layoutChanged(QListWidgetItem* current, QListWidgetItem* previous);

public:
    Ui::RegexProcessorDialog* ui;

private slots:
    void on_lineEditTestSource_textEdited(const QString &arg1);
    void on_comboBoxFormat_currentTextChanged(const QString &arg1);
    void on_pushButtonEditFormatScheme_clicked();
    void on_pushButtonOk_clicked();
    void on_checkBoxOnlyOutputMatches_toggled(bool checked);
    void on_testRegexComboBox_editTextChanged(const QString &arg1);
    void on_testRegexComboBox_currentIndexChanged(int index);

private:
    RegexProcessor* m_regexProcessor = nullptr;
    int m_nofActiveCells = 0;
    AntiLog* m_antiLog;
};

