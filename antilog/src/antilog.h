#pragma once

#include "inputlist.h"
#include "logentry.h"

#include <QObject>
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

    enum CopyFormat
    {
        TEXT,
        HTML
    };

    bool eventFilter(QObject *obj, QEvent *event);
    void copySelectionToClipboard(AntiLog::CopyFormat format);
    void adjustColumnWidth(int width);
    void refreshLogView();

public slots:
    void slotFormatRuleChanged();
    void slotNewLogEntry(InputItemBase* processor, LogEntryPtr logEntry);

private slots:
    void slotShowContextMenu(const QPoint &pos);
    void slotCopyTextToClipboard();
    void slotCopyHtmlToClipboard();

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
    void on_pushButtonClear_clicked();

private:
    Ui::MainWindow *ui;
    LogViewTableModel* m_logViewTableModel = nullptr;
    InputDialog* m_inputDialog = nullptr;
    InputList m_inputList;
    bool m_scrollToBottom = true;
    int m_columnWidth = 0;
};

