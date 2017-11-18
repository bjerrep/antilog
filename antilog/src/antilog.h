#pragma once

#include "inputlist.h"
#include "logentry.h"

#include <QObject>
#include <QWidget>

class InputDialog;
class InputTableViewModel;
class LogSeverityCategories;
class InputItemBase;
class LogViewTableModel;
class ExtendedFilterModel;
class SourceBase;

namespace Ui
{
class MainWindow;
}

class AntiLog : public QWidget
{
    Q_OBJECT

public:
    explicit AntiLog(QWidget* parent = nullptr);
    ~AntiLog();

    QVector<InputItemBase*> getAllSourcesAndProcessors() const;

private:
    void save() const;
    void load();
    static QString getConfigFilePath();

    enum CopyFormat
    {
        TEXT,
        HTML
    };

    void keyPressEvent(QKeyEvent* keyEvent);
    void copySelectionToClipboard(AntiLog::CopyFormat format);
    void deleteSelection();
    void adjustColumnWidth(int width);
    void refreshLogView();
    void timerEvent(QTimerEvent*);
    void updateSourceTrafficMeter();
    void setupExtendedFilters();

public slots:
    void slotFormatRuleChanged();
    void slotNewLogEntry(InputItemBase* processor, LogEntryPtr logEntry);
    void slotSourceTrafficMonitor(SourceBase*, QString, QString);

private slots:
    void slotShowContextMenu(const QPoint& pos);
    void slotCopyTextToClipboard();
    void slotCopyHtmlToClipboard();
    void slotTableUpdated();
    void slotLogViewSliderChanged(int);
    void slotInputWidgetClosed();
    void slotExtendedFiltersModified();
    void slotDeletingModelRows(int count);

    void closeEvent (QCloseEvent* event);
    void on_checkBoxScroll_clicked(bool checked);
    void on_pushButtonFormat_clicked();
    void on_setupButton_clicked();
    void on_pushButtonInputs_clicked();
    void on_comboBoxLogThreshold_currentIndexChanged(const QString& arg1);
    void on_lineEditTextFilter_textChanged(const QString& arg1);
    void on_checkBoxShowSource_clicked(bool checked);
    void on_pushButtonClear_clicked();
    void on_pushButtonFilterDialog_clicked();
    void on_checkBoxUseFilters_clicked(bool checked);

private:
    Ui::MainWindow* ui;
    LogViewTableModel* m_logViewTableModel = nullptr;
    ExtendedFilterModel* m_extendedFilterModel = nullptr;
    InputDialog* m_inputDialog = nullptr;
    InputList m_inputList;
    bool m_scrollToBottom = true;
    int m_columnWidth = 0;
    int m_sourceTrafficValue = 0;
    bool m_useExtendedFilters = false;
    int m_modelRowsDeleted = -1;
    int m_firstRowOnDisplay = 0;
    QString m_version;
};

