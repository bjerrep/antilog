#pragma once

#include "inputlist.h"
#include "logentry.h"

#include <QWidget>

class InputDialog;
class InputTableViewModel;
class LogLevelCategories;
class InputItemBase;
class LogViewTableModel;

namespace Ui
{
class MainWindow;
}

class AntiLog : public QWidget
{
    Q_OBJECT

public:
    explicit AntiLog(QWidget *parent = nullptr);
    ~AntiLog();

    void save() const;
    void load();
    static QString getConfigFilePath();

public slots:
    void slotFormatRuleChanged();
    void slotNewLogEntry(InputItemBase* processor, LogEntryPtr logEntry);

private slots:
    void slotTableUpdated();
    void slotLogViewSliderChanged(int);
    void inputWidgetClosed();

    void closeEvent (QCloseEvent* event);
    void on_checkBoxScroll_clicked(bool checked);
    void on_pushButtonFormat_clicked();
    void on_setupButton_clicked();
    void on_pushButtonInputs_clicked();
    void on_comboBoxLogThreshold_currentIndexChanged(const QString &arg1);
    void on_lineEditTextFilter_textChanged(const QString &arg1);
    void on_checkBoxShowSource_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    LogViewTableModel* m_logViewTableModel = nullptr;
    InputDialog* m_inputDialog = nullptr;
    InputList m_inputList;
    bool m_scrollToBottom = true;
};
