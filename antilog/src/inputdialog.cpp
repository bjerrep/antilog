#include "inputdialog.h"
#include "ui_inputdialog.h"
#include "newinputdialog.h"
#include "statics.h"
#include "inputsources.h"
#include "inputtableviewmodel.h"
#include "sourcevisitors.h"
#include "sourceandprocessor.h"
#include "inputlist.h"
#include "antilog.h"

#include <QAbstractItemView>
#include <QSpinBox>
#include <QRadioButton>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QMenu>
#include <QListWidgetItem>
#include <QTableView>
#include <QWidget>
#include <QHeaderView>


InputDialog::InputDialog(InputList* inputList, AntiLog* ulw) :
    QDialog(ulw),
    ui(new Ui::inputDialog),
    m_app(ulw)
{
    setObjectName("Input configuration");
    setAttribute( Qt::WA_DeleteOnClose );
    ui->setupUi(this);

    m_inputList = inputList;

    m_inputTableViewModel = new InputTableViewModel(inputList, ulw);
    ui->tableView->setModel(m_inputTableViewModel);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    redrawTable();

    // vertical size
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setDefaultSectionSize(Statics::iconHeight + 2 * Statics::verticalMargin);

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotShowContextMenu(QPoint)));
}

InputDialog::~InputDialog()
{
    delete m_inputTableViewModel;
    delete ui;
}

int InputDialog::insertInputItemWidget(int row, int column, InputItemBase* inputItem)
{
    auto index = m_inputTableViewModel->index(row, column);
    auto* getWidget = new GetInputDialogWidget();
    inputItem->accept(getWidget);
    ui->tableView->setIndexWidget(index, getWidget);
    int width = getWidget->width();
    ui->tableView->setColumnWidth(column, width);
    return width;
}

int InputDialog::addImageToTable(int row, int column, QString imageResource)
{
    auto index = m_inputTableViewModel->index(row, column);
    auto* label = new QLabel();
    label->setAlignment(Qt::AlignCenter);
    label->setPixmap(Statics::pixmapIcon(imageResource));
    int width = label->pixmap()->width() + 10;
    ui->tableView->setIndexWidget(index, label);
    ui->tableView->setColumnWidth(column, width);
    return width;
}

int InputDialog::redrawRow(int row)
{
    int widthSum = 0;

    widthSum += insertInputItemWidget(row,
                                Column::Source,
                                m_inputList->getSourceAndProcessor(row)->getSourceEntry());

    widthSum += addImageToTable(row, Column::Arrow, ":/artwork/artwork/arrow.svg");

    widthSum += insertInputItemWidget(row,
                                Column::Processor,
                                m_inputList->getSourceAndProcessor(row)->getProcessorEntry());

    bool isOn = m_inputList->getSourceAndProcessor(row)->isOn();

    widthSum += addImageToTable(row, Column::Enable, isOn ?
                             ":/artwork/artwork/on.svg" :
                             ":/artwork/artwork/off.svg");

    for(int i = 0; i < m_inputTableViewModel->columnCount(); i++)
    {
        QModelIndex index = m_inputTableViewModel->index(row, i);
        ui->tableView->indexWidget(index)->setEnabled(isOn);
    }

    widthSum += ui->tableView->verticalScrollBar()->width();

    return widthSum;
}

void InputDialog::redrawTable()
{
    int width = 0;

    for(int i = 0; i < m_inputList->count(); i++)
    {
        width = redrawRow(i);
    }

    ui->tableView->setMinimumWidth(width + 4); // + 1 per row
    m_inputTableViewModel->layoutChanged();
}

void InputDialog::slotShowContextMenu(const QPoint &pos)
{
    auto globalPos = ui->tableView->mapToGlobal(pos);
    QMenu contextMenu;
    contextMenu.addAction("Delete", this, SLOT(slotDeleteInput()));
    contextMenu.addAction("Source type", this, SLOT(slotChangeSource()));
    contextMenu.addAction("Edit source", this, SLOT(slotEditSource()));
    contextMenu.addAction("Processor type", this, SLOT(slotChangeProcessor()));
    contextMenu.addAction("Edit processor", this, SLOT(slotEditProcessor()));
    contextMenu.exec(globalPos);
}

QModelIndex InputDialog::getSelectedModelIndex(int column) const
{
    auto rows = ui->tableView->selectionModel()->selectedRows();
    return m_inputTableViewModel->index(rows[0].row(), column);
}

void InputDialog::slotDeleteInput()
{
    auto index = getSelectedModelIndex(0);

    m_inputTableViewModel->removeRow(index.row());
}

void InputDialog::slotChangeSource()
{
}

void InputDialog::slotChangeProcessor()
{
}

void InputDialog::slotEditSource()
{
    auto index = getSelectedModelIndex(Column::Source);
    emit on_tableView_doubleClicked(index);
}

void InputDialog::slotEditProcessor()
{
    auto index = getSelectedModelIndex(Column::Processor);

    emit on_tableView_doubleClicked(index);
}

void InputDialog::on_tableView_doubleClicked(const QModelIndex& index )
{
    switch(index.column())
    {
    case Column::Source:
    {
        auto* sourceEntry = m_inputList->getSourceAndProcessor(index.row())->getSourceEntry();
        GetDialog getDialog;
        sourceEntry->accept(&getDialog);
        break;
    }
    case Column::Processor:
    {
        auto* sourceEntry = m_inputList->getSourceAndProcessor(index.row())->getProcessorEntry();
        GetDialog getDialog;
        sourceEntry->accept(&getDialog);
        break;
    }
    case Column::Enable:
    {
        m_inputList->getSourceAndProcessor(index.row())->toggleEnable();
        break;
    }
    }
    redrawTable();
}

void InputDialog::on_pushButtonNewInput_clicked()
{
    InputItemList sources;
    getAllSources(sources);
    InputItemList processors;
    getAllProcessors(processors);

    auto newInputDialog = new NewInputDialog(sources, processors, this);

    if (newInputDialog->exec() == QDialog::Accepted)
    {
        int sourceRow = newInputDialog->currentSourceIndex();
        int processorRow = newInputDialog->currentProcessorIndex();

        auto sap = new SourceAndProcessor(sources.takeAt(sourceRow), processors.takeAt(processorRow));
        m_inputTableViewModel->append(sap);
        ui->tableView->selectRow(m_inputTableViewModel->rowCount() - 1);

        redrawTable();
    }
    qDeleteAll(sources);
    qDeleteAll(processors);
}

void InputDialog::on_pushButtonClose_clicked()
{
    close();
    setResult(QDialog::Accepted);
}
