#pragma once

#include <QDialog>

class FormatScheme;
class FormatWidget;
class InputList;
class FormatSchemes;

namespace Ui {
class FormatDialog;
}

class FormatDialog : public QDialog
{
    Q_OBJECT

public:
    FormatDialog(const QString& schemeName,
                 QWidget* parent,
                 const InputList* inputList = nullptr);
    ~FormatDialog();

private:
    void setSchemeFromNameAndRedraw(const QString& schemeName);

private slots:
    void redraw();
    void slotDeleteWidget(FormatWidget* sender);
    void slotFormatRuleChanged();
    void updateFormatModel();

    void on_comboBoxSchemeName_currentTextChanged(const QString& arg1);
    void on_pushButtonClose_clicked();
    void on_pushButtonNewScheme_clicked();
    void on_pushButtonDeleteScheme_clicked();
    void on_pushButtonAddRule_clicked();

signals:
    void signalFormatRuleChanged();

private:
    Ui::FormatDialog* ui;
    FormatScheme* m_formatScheme = nullptr;
    const InputList* m_inputList = nullptr;
    FormatSchemes* m_formatSchemeModel = nullptr;
};
