#pragma once
#include "tableformat.h"
#include <QWidget>

class FormatRule;

namespace Ui {
class FormatWidget;
}

class FormatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FormatWidget(FormatRule* formatScheme, const TableFormat& tableFormat, QWidget *parent);
    ~FormatWidget();

    FormatRule* formatEntry();

signals:
    void signalDeleteWidget(FormatWidget*);
    void signalFormatRuleChanged();

private slots:
    void on_comboBoxScope_currentTextChanged(const QString &arg1);
    void on_comboBoxOperation_currentIndexChanged(const QString &arg1);
    void on_comboBoxSearchTerm_editTextChanged(const QString &arg1);
    void on_checkBoxBold_clicked(bool checked);
    void on_pushButtonDelete_clicked();
    void on_comboBoxColor_currentTextChanged(const QString &arg1);
    void on_comboBoxBgcolor_currentIndexChanged(const QString &arg1);
    void on_comboBoxSelection_currentTextChanged(const QString &arg1);
    void on_checkBoxCase_clicked(bool checked);

private:
    Ui::FormatWidget *ui;
    FormatRule* m_formatRule;
};

