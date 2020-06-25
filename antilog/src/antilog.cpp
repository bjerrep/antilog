#include "antilog.h"
#include "ui_antilog.h"
#include "inputs/inputdialog.h"
#include "inputs/sourceandprocessor.h"
#include "inputs/inputsources.h"
#include "optionsdialog.h"
#include "extendedfilter/extendedfilterdialog.h"
#include "extendedfilter/extendedfiltermodel.h"
#include "format/formatdialog.h"
#include "format/formatschememodel.h"
#include "logentry/logseverities.h"
#include "logentry/logviewtablemodel.h"
#include "column/columndialog.h"

#include <QJsonDocument>
#include <QScrollBar>
#include <QStandardPaths>
#include <QDir>
#include <QMenu>
#include <QClipboard>
#include <QTableView>
#include <QProgressBar>
#include <QKeyEvent>
#include <QMessageBox>
#include <QHostAddress>


AntiLog::AntiLog(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    m_inputList(this)
{
    ui->setupUi(this);
    load();

    setFont(Statics::instOptions()->m_appFont);
    m_logViewTableModel = new LogViewTableModel(this, m_extendedFilterModel);
    connect(m_logViewTableModel, &LogViewTableModel::newEntriesAdded,
            this, &AntiLog::slotTableUpdated);
    connect(m_logViewTableModel, &LogViewTableModel::signalDeletingRows,
            this, &AntiLog::slotDeletingModelRows);
    ui->tableView->setModel(m_logViewTableModel);
    ui->tableView->setItemDelegate(new LogViewDelegate(this));
    connect(ui->tableView->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &AntiLog::slotLogViewSliderChanged);

    ui->tableView->verticalHeader()->setDefaultSectionSize(
                Statics::instOptions()->m_logFontHeight + Statics::LogViewMargin);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, &QTableView::customContextMenuRequested,
            this, &AntiLog::slotShowContextMenu);

    ui->checkBoxScroll->setChecked(m_scrollToBottom);
    ui->checkBoxShowSource->setChecked(Statics::instOptions()->m_showSource);

    QString currentLogSeverity = Statics::instOptions()->m_logThreshold;
    ui->comboBoxLogThreshold->clear();
    ui->comboBoxLogThreshold->addItems(Statics::s_logSeverities->getCategoryNames());
    ui->comboBoxLogThreshold->setCurrentText(currentLogSeverity);

    setupExtendedFilters();

    startTimer(400);
    updateSourceTrafficMeter();
}

AntiLog::~AntiLog()
{
    delete ui;
    delete m_extendedFilterModel;

    delete Statics::instOptions();
    delete m_formatSchemes;
    delete Statics::s_logSeverities;
    delete Statics::m_globalColumnConfig;
}

void AntiLog::save() const
{
    QJsonObject json;
    m_inputList.save(json);
    m_formatSchemes->save(json);
    Statics::s_logSeverities->save(json);
    Statics::instOptions()->save(json);
    m_extendedFilterModel->save(json);

    json["useextendedfilters"] = ui->checkBoxUseFilters->isChecked();
    Statics::instColumnLibrary()->save(json);
    json["width"] = geometry().width();
    json["height"] = geometry().height();
    json["x"] = geometry().topLeft().x();
    json["y"] = geometry().topLeft().y();
    json["version"] = "devel";

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

    Statics::s_antiLog = this;

    Statics::s_options = new Options(json);
    Statics::m_globalColumnConfig = new GlobalColumnConfig(json);
    connect(Statics::instOptions(), &Options::signalInvalidated,
            this, &AntiLog::slotRedrawLogView,
            Qt::QueuedConnection);

    m_formatSchemes = new FormatSchemes(json, Statics::instColumnLibrary());
    Statics::s_logSeverities = new LogSeverityCategories(json);
    m_extendedFilterModel = new ExtendedFilterModel(json);
    connect(m_extendedFilterModel, &ExtendedFilterModel::signalExtendedFiltersModified,
            this, &AntiLog::slotExtendedFiltersModified);

    m_inputList.load(json);

    if (!json.empty())
    {
        m_useExtendedFilters = json["useextendedfilters"].toBool();
        int w = json["width"].toInt();
        int h = json["height"].toInt();
        int x = json["x"].toInt();
        int y = json["y"].toInt();
        m_version = json["version"].toString();
        setGeometry(x, y, w, h);
    }
}

InputItemVector AntiLog::getAllSourcesAndProcessors() const
{
    return m_inputList.getAllSourcesAndProcessors();
}

// a way for a host app to dynamically configure all udp sources. This is
// the first go at something that should really be part of an official antilog api.
// Time will tell if that happens.
void AntiLog::reconfigureUdpSources(QString address, uint16_t port)
{
    auto host_address = QHostAddress(address);
    for(int i=0; i<m_inputList.count(); i++)
    {
        UDPSource* source = dynamic_cast<UDPSource*>(m_inputList.getSourceAndProcessor(i)->getSourceEntry());
        if (source)
        {
            source->setAddress(address);
            source->setPort(port);
        }
    }
}

FormatSchemes* AntiLog::getFormatSchemeModel()
{
    return m_formatSchemes;
}

GlobalColumnConfig *AntiLog::getGlobalColumnConfig()
{
    return Statics::m_globalColumnConfig;
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

void AntiLog::keyPressEvent(QKeyEvent* keyEvent)
{
    if (keyEvent->key() == Qt::Key_C && (keyEvent->modifiers().testFlag(Qt::ControlModifier)))
    {
        copySelectionToClipboard(TEXT);
        return;
    }
    else if (keyEvent->key() == Qt::Key_Delete)
    {
        deleteSelection();
        return;
    }
    QWidget::keyPressEvent(keyEvent);
}

/// Automatically enable scroll-to-end if the user scrolled to the last model row
/// and automatically disable scroll-to-end if last model row isn't visible.
///
void AntiLog::slotLogViewSliderChanged(int value)
{
    // When entries are to be deleted in the model calling beginRemoveRows() triggers
    // a chain that ends here since the table view is reset to display the first model entry.
    // The root cause or correct fix remains to be found, this is just creative hacking around.
    if (value == m_modelRowsDeleted)
    {
        if (m_scrollToBottom)
        {
            ui->tableView->scrollToBottom();
        }
        else
        {
            m_firstRowOnDisplay -= m_modelRowsDeleted;
            ui->tableView->scrollTo(m_logViewTableModel->index(m_firstRowOnDisplay, 0), QAbstractItemView::PositionAtTop);
        }
        return;
    }

    QPoint firstRowPoint = QPoint(0, 2);
    m_firstRowOnDisplay = ui->tableView->indexAt(firstRowPoint).row();

    int lastRowFromBottomOmgThisCantBeRightOffset =
            ui->tableView->rowHeight(m_firstRowOnDisplay) / 2 +
            ui->tableView->horizontalScrollBar()->height();

    QPoint lastRowPoint = ui->tableView->rect().bottomLeft() -
            QPoint(0, lastRowFromBottomOmgThisCantBeRightOffset);
    int lastRowOnDisplay = ui->tableView->indexAt(lastRowPoint).row();
    int rowsOnDisplay = lastRowOnDisplay - m_firstRowOnDisplay + 1;
    int rowsInModel = ui->tableView->model()->rowCount();

    if (value < rowsInModel - rowsOnDisplay)
    {
        if (m_scrollToBottom)
        {
            m_scrollToBottom = false;
            ui->checkBoxScroll->setChecked(m_scrollToBottom);
        }
    }
    else if (value >= m_firstRowOnDisplay)
    {
        if (!m_scrollToBottom)
        {
            m_scrollToBottom = true;
            ui->checkBoxScroll->setChecked(m_scrollToBottom);
        }
    }
}

void AntiLog::slotDeletingModelRows(int count)
{
    m_modelRowsDeleted = count;
}

void AntiLog::slotTableUpdated()
{
    if (m_scrollToBottom)
    {
        ui->tableView->scrollToBottom();
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

void AntiLog::on_checkBoxScroll_clicked(bool checked)
{
    m_scrollToBottom = checked;
    if (checked)
    {
        ui->tableView->scrollToBottom();
    }
    else
    {
        QPoint firstRowPoint = QPoint(0, 6);
        m_firstRowOnDisplay = ui->tableView->indexAt(firstRowPoint).row();
    }
}

void AntiLog::on_pushButtonInputs_clicked()
{
    if (!m_inputDialog)
    {
        m_inputDialog = new InputDialog(&m_inputList, this);
        connect(m_inputDialog, &InputDialog::destroyed, this, &AntiLog::slotInputWidgetClosed);
        m_inputDialog->exec();
    }
}

void AntiLog::slotNewLogEntry(InputItemBase* /*processor*/, LogEntryPtr logEntry)
{
    adjustColumnWidth(logEntry->getWidth());
    m_extendedFilterModel->registerLogEntry(logEntry);
    m_logViewTableModel->append(logEntry);
}

void AntiLog::refreshLogView()
{
    m_logViewTableModel->redrawVisibleRows();
    slotTableUpdated();
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
    m_extendedFilterModel->setActive(m_useExtendedFilters);
}

void AntiLog::slotInputWidgetClosed()
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
    contextMenu.addAction("Info about this entry", this, &AntiLog::slotLogEntryInfo);
    contextMenu.addAction("As text", this, &AntiLog::slotCopyTextToClipboard);
    contextMenu.addAction("As html", this, &AntiLog::slotCopyHtmlToClipboard);
    contextMenu.exec(globalPos);
}

void AntiLog::slotLogEntryInfo()
{
    int index = ui->tableView->selectionModel()->currentIndex().row();
    auto logEntryPtr = static_cast<LogEntryPtr>(ui->tableView->model()->index(index, 0).data().value<LogEntryPtr>());

    QMessageBox::information(this, "Logentry",
                             logEntryPtr->toString(),
                             QMessageBox::Ok);
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

void AntiLog::deleteSelection()
{
    QVector<std::array<int, 2>> m_set;

    for (int i = 0; i < m_logViewTableModel->rowCount(); i++)
    {
        if (ui->tableView->selectionModel()->isRowSelected(i, QModelIndex()))
        {
            int first = i;
            int last = i;
            for (;
                 last < m_logViewTableModel->rowCount() && ui->tableView->selectionModel()->isRowSelected(last, QModelIndex());
                 last++);
            int count = last - first;
            m_set.append({first, count});
            i = last;
        }
    }

    while (!m_set.isEmpty())
    {
        auto pair = m_set.takeLast();
        m_logViewTableModel->removeRows(pair[0], pair[1]);
    }
}

void AntiLog::on_setupButton_clicked()
{
    OptionsDialog optionsDialog(*Statics::s_options, this, Statics::instColumnLibrary());
    optionsDialog.exec();
}

void AntiLog::on_pushButtonFormat_clicked()
{
    QString schemeToLoad = m_formatSchemes->getSchemeNames().back();
    FormatDialog formatDialog(schemeToLoad, this, &m_inputList);

    connect(&formatDialog, &FormatDialog::signalFormatRuleChanged,
            this, &AntiLog::slotRedrawLogView);

    formatDialog.exec();
}

void AntiLog::slotRedrawLogView()
{
    m_logViewTableModel->redrawVisibleRows();
}

void AntiLog::on_comboBoxLogThreshold_currentIndexChanged(const QString& arg1)
{
    Statics::s_options->m_logThreshold = arg1;
    m_logViewTableModel->newLogSeverity(arg1);
}

void AntiLog::on_lineEditTextFilter_textChanged(const QString& arg1)
{
    m_logViewTableModel->setTextFilter(arg1);
    refreshLogView();
}

void AntiLog::on_checkBoxShowSource_clicked(bool checked)
{
    Statics::s_options->m_showSource = checked;
    m_logViewTableModel->redrawVisibleRows();
}

void AntiLog::on_pushButtonClear_clicked()
{
    m_logViewTableModel->clear();
    adjustColumnWidth(0);
}

void AntiLog::on_pushButtonFilterDialog_clicked()
{
    ExtendedFilterDialog dialog(this, m_extendedFilterModel);
    dialog.exec();
}

void AntiLog::on_checkBoxUseFilters_clicked(bool checked)
{
    m_useExtendedFilters = checked;
    setupExtendedFilters();
}

void AntiLog::slotExtendedFiltersModified()
{
    m_logViewTableModel->redrawVisibleRows();
}
