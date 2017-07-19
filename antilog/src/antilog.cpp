#include "antilog.h"
#include "ui_antilog.h"
#include "inputdialog.h"
#include "optionsdialog.h"
#include "formatdialog.h"
#include "extendedfilterdialog.h"
#include "extendedfiltermodel.h"
#include "formatschememodel.h"
#include "loglevels.h"
#include "logviewtablemodel.h"

#include <QJsonDocument>
#include <QScrollBar>
#include <QStandardPaths>
#include <QDir>
#include <QMenu>
#include <QClipboard>
#include <QTableView>
#include <QProgressBar>

AntiLog::AntiLog(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    m_inputList(this)
{
    load();

    ui->setupUi(this);

    setFont(Statics::options->m_appFont);
    m_logViewTableModel = new LogViewTableModel(this, m_filterModel);
    connect(m_logViewTableModel, &LogViewTableModel::newEntriesAdded,
            this, &AntiLog::slotTableUpdated);
    ui->tableView->setModel(m_logViewTableModel);
    ui->tableView->setItemDelegate(new LogViewDelegate());
    connect(ui->tableView->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(slotLogViewSliderChanged(int)));

    ui->tableView->verticalHeader()->setDefaultSectionSize(
                Statics::options->m_logFontHeight + Statics::LogViewMargin);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotShowContextMenu(QPoint)));

    ui->checkBoxScroll->setChecked(m_scrollToBottom);
    ui->checkBoxShowSource->setChecked(Statics::options->m_showSource);

    QString currentLogLevel = Statics::options->m_logThreshold;
    ui->comboBoxLogThreshold->clear();
    ui->comboBoxLogThreshold->addItems(Statics::logLevels->getCategoryNames());
    ui->comboBoxLogThreshold->setCurrentText(currentLogLevel);

    setupExtendedFilters();

    installEventFilter(this);

    startTimer(400);
    updateSourceTrafficMeter();
}

AntiLog::~AntiLog()
{
    delete ui;
    delete m_filterModel;
}

void AntiLog::save() const
{
    QJsonObject json;
    m_inputList.save(json);
    Statics::formatSchemeModel->save(json);
    Statics::logLevels->save(json);
    Statics::options->save(json);
    m_filterModel->save(json);

    json["useextendedfilters"] = ui->checkBoxUseFilters->isChecked();

    QFile file(getConfigFilePath());
    if (file.open(QIODevice::WriteOnly))
    {
        QJsonDocument doc(json);
        file.write(doc.toJson());
    }
}

void AntiLog::load()
{
    QFile file(getConfigFilePath());
    if (!file.open(QIODevice::ReadOnly))
    {
        warn("unable to open " + getConfigFilePath());
    }
    QJsonDocument doc(QJsonDocument::fromJson(file.readAll()));
    auto json = doc.object();

    Statics::antiLog = this;
    Statics::options = new Options(json);
    Statics::formatSchemeModel = new FormatSchemeModel(json);
    Statics::logLevels = new LogLevelCategories(json);
    m_filterModel = new ExtendedFilterModel(json);

    m_inputList.load(json);

    if (!json.empty())
    {
        m_useExtendedFilters = json["useextendedfilters"].toBool();
    }
}

QString AntiLog::getConfigFilePath()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(path);
    return QDir(path).filePath("config.json");
}

void AntiLog::closeEvent (QCloseEvent*)
{
    if (m_inputDialog)
    {
        m_inputDialog->close();
    }
    save();
}

void AntiLog::on_checkBoxScroll_clicked(bool checked)
{
    m_scrollToBottom = checked;
    if (checked)
    {
        ui->tableView->scrollToBottom();
    }
}

bool AntiLog::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::ShortcutOverride)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->matches(QKeySequence::Copy) && ui->tableView->model()->rowCount())
        {
            copySelectionToClipboard(HTML);
            return true;
        }
    }
    return false;
}

/// Automatically enable scroll-to-end if the user scrolled to the last row
/// and automatically disable scroll-to-end if the view is scrolled upwards.
///
void AntiLog::slotLogViewSliderChanged(int value)
{
    if (value == 1 && m_scrollToBottom)
    {
        // ignore and revert, this callback comes when rows are removed in the model
        ui->tableView->scrollToBottom();
        return;
    }

    QPoint firstRowPoint = QPoint(0, 6);
    int firstRowOnDisplay = ui->tableView->indexAt(firstRowPoint).row();

    int lastRowFromBottomOmgThisCantBeRightOffset =
            ui->tableView->rowHeight(firstRowOnDisplay) / 2 +
            ui->tableView->horizontalScrollBar()->height();

    QPoint lastRowPoint = ui->tableView->rect().bottomLeft() -
            QPoint(0, lastRowFromBottomOmgThisCantBeRightOffset);
    int lastRowOnDisplay = ui->tableView->indexAt(lastRowPoint).row();
    int rowsOnDisplay = lastRowOnDisplay - firstRowOnDisplay + 1;
    int rowsInModel = ui->tableView->model()->rowCount();

    if (value < rowsInModel - rowsOnDisplay)
    {
        if (m_scrollToBottom)
        {
            m_scrollToBottom = false;
            ui->checkBoxScroll->setChecked(m_scrollToBottom);
        }
    }
    else if (value >= firstRowOnDisplay)
    {
        if (!m_scrollToBottom)
        {
            m_scrollToBottom = true;
            ui->checkBoxScroll->setChecked(m_scrollToBottom);
        }
    }
}

void AntiLog::adjustColumnWidth(int width)
{
    if (!width || width > m_columnWidth)
    {
        m_columnWidth = width;
        ui->tableView->horizontalHeader()->setMinimumSectionSize(width);
    }
}

void AntiLog::on_pushButtonInputs_clicked()
{
    if (!m_inputDialog)
    {
        m_inputDialog = new InputDialog(&m_inputList, this);
        connect(m_inputDialog, &InputDialog::destroyed, this, &AntiLog::inputWidgetClosed);
        m_inputDialog->exec();
    }
}

void AntiLog::slotNewLogEntry(InputItemBase* /*processor*/, LogEntryPtr logEntry)
{
    adjustColumnWidth(logEntry->getWidth());
    m_filterModel->registerLogEntry(logEntry);
    m_logViewTableModel->append(logEntry);
}

void AntiLog::refreshLogView()
{
    m_logViewTableModel->redrawVisibleRows();
    slotTableUpdated();
}

void AntiLog::slotTableUpdated()
{
    if (m_scrollToBottom)
    {
        ui->tableView->scrollToBottom();
    }
}

void AntiLog::updateSourceTrafficMeter()
{
    int maximum = ui->progressBarTraffic->maximum();
    ui->progressBarTraffic->setValue(m_sourceTrafficValue > maximum ? maximum : m_sourceTrafficValue);
}

void AntiLog::slotSourceTrafficMonitor(SourceBase*, QString, QString)
{
    int maximum = ui->progressBarTraffic->maximum();
    m_sourceTrafficValue += maximum;
    m_sourceTrafficValue = m_sourceTrafficValue > maximum + 2 ? maximum + 2 : m_sourceTrafficValue;
    updateSourceTrafficMeter();
}

void AntiLog::timerEvent(QTimerEvent*)
{
    if (m_sourceTrafficValue > 0)
        --m_sourceTrafficValue;
    updateSourceTrafficMeter();
}

void AntiLog::setupExtendedFilters()
{
    ui->checkBoxUseFilters->setChecked(m_useExtendedFilters);
    ui->pushButtonFilterDialog->setEnabled(m_useExtendedFilters);
    m_filterModel->setActive(m_useExtendedFilters);
}

void AntiLog::inputWidgetClosed()
{
    m_inputDialog = nullptr;
}

void AntiLog::slotShowContextMenu(const QPoint& pos)
{
    if (!ui->tableView->model()->rowCount())
    {
        return;
    }
    auto globalPos = ui->tableView->mapToGlobal(pos);
    QMenu contextMenu;
    contextMenu.addSection("Copy selection");
    contextMenu.addAction("As text", this, &AntiLog::slotCopyTextToClipboard);
    contextMenu.addAction("As html", this, &AntiLog::slotCopyHtmlToClipboard);
    contextMenu.exec(globalPos);
}

void AntiLog::slotCopyTextToClipboard()
{
    copySelectionToClipboard(TEXT);
}

void AntiLog::slotCopyHtmlToClipboard()
{
    copySelectionToClipboard(HTML);
}

void AntiLog::copySelectionToClipboard(CopyFormat format)
{
    QString data;

    for (int i = 0; i < ui->tableView->model()->rowCount(); i++)
    {
        if (!ui->tableView->selectionModel()->isRowSelected(i, QModelIndex()))
        {
            continue;
        }
        auto logEntryPtr = static_cast<LogEntryPtr>(ui->tableView->model()->index(i, 0).data().value<LogEntryPtr>());
        switch (format)
        {
        case HTML:
            data += logEntryPtr->getHtml();
            break;
        case TEXT:
            data += logEntryPtr->getText() + '\n';
            break;
        }
    }

    if (data.count())
    {
        QApplication::clipboard()->setText(data);
    }
}

void AntiLog::on_setupButton_clicked()
{
    OptionsDialog optionsDialog(*Statics::options, this);
    optionsDialog.exec();
}

void AntiLog::on_pushButtonFormat_clicked()
{
    FormatDialog formatDialog(Statics::NoneScheme, this, &m_inputList);
    formatDialog.exec();
}

void AntiLog::slotFormatRuleChanged()
{
    m_logViewTableModel->redrawVisibleRows();
}

void AntiLog::on_comboBoxLogThreshold_currentIndexChanged(const QString& arg1)
{
    Statics::options->m_logThreshold = arg1;
    m_logViewTableModel->newLogLevel(arg1);
}

void AntiLog::on_lineEditTextFilter_textChanged(const QString& arg1)
{
    m_logViewTableModel->setTextFilter(arg1);
    refreshLogView();
}

void AntiLog::on_checkBoxShowSource_clicked(bool checked)
{
    Statics::options->m_showSource = checked;
    m_logViewTableModel->redrawVisibleRows();
}

void AntiLog::on_pushButtonClear_clicked()
{
    m_logViewTableModel->clear();
    adjustColumnWidth(0);
}

void AntiLog::on_pushButtonFilterDialog_clicked()
{
    ExtendedFilterDialog dialog(this, m_filterModel);
    dialog.exec();
}

void AntiLog::on_checkBoxUseFilters_clicked(bool checked)
{
    m_useExtendedFilters = checked;
    setupExtendedFilters();
}
