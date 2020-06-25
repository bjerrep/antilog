#include "inputdialog.h"
#include "ui_inputdialog.h"
#include "ui_inputwidget.h"
#include "newinputdialog.h"
#include "inputtableviewmodel.h"
#include "sourcevisitors.h"
#include "sourceandprocessor.h"
#include "inputlist.h"
#include "antilog.h"

#include <QScrollBar>
#include <QMenu>
#include <QTableView>
#include <QHeaderView>
#include <QResizeEvent>
#include <QLabel>

InputTableView::InputTableView(QWidget *parent) : QTableView(parent)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    horizontalHeader()->hide();
    verticalHeader()->hide();
}

void InputTableView::setImageWidths(int width)
{
    m_imageWidths = width;
}

void InputTableView::resizeEvent(QResizeEvent *event)
{
    int width = (event->size().width() - 2 * m_imageWidths) / 2;
    setColumnWidth(0, width);
    setColumnWidth(1, m_imageWidths);
    setColumnWidth(2, width);
    setColumnWidth(3, m_imageWidths);
    QTableView::resizeEvent(event);
}



InputDialog::InputDialog(InputList* inputList, AntiLog* antiLog) :
    QDialog(antiLog),
    ui(new Ui::inputDialog),
    m_antiLog(antiLog)
{
    setObjectName("Input configuration");
    setAttribute( Qt::WA_DeleteOnClose );
    ui->setupUi(this);

    m_inputList = inputList;

    m_inputTableViewModel = new InputTableViewModel(inputList, antiLog);
    m_tableView = new InputTableView(this);
    ui->verticalLayout->insertWidget(0, m_tableView);
    m_tableView->setModel(m_inputTableViewModel);

    redrawTable();

    connect(m_tableView, &InputTableView::doubleClicked, this, &InputDialog::on_tableView_doubleClicked);

    // vertical size
    m_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->verticalHeader()->setDefaultSectionSize(
                Statics::InputIconHeight +
                2 * Statics::InputIconVerticalMargin);

    m_tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotShowContextMenu(QPoint)));
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
    m_tableView->setIndexWidget(index, getWidget);
    int width = getWidget->width();
    return width;
}

int InputDialog::addImageToTable(int row, int column, QString imageResource)
{
    auto index = m_inputTableViewModel->index(row, column);
    auto* label = new QLabel();
    label->setAlignment(Qt::AlignCenter);
    label->setPixmap(Statics::pixmapIcon(imageResource));
    int width = label->pixmap()->width() + 10;
    m_tableView->setIndexWidget(index, label);
    return width;
}

void InputDialog::redrawRow(int row)
{
    insertInputItemWidget(row,
                          Column::Source,
                          m_inputList->getSourceAndProcessor(row)->getSourceEntry());

    addImageToTable(row, Column::Arrow, ":/artwork/artwork/arrow.svg");

    insertInputItemWidget(row,
                          Column::Processor,
                          m_inputList->getSourceAndProcessor(row)->getProcessorEntry());

    bool isOn = m_inputList->getSourceAndProcessor(row)->isOn();

    int width = addImageToTable(row, Column::Enable, isOn ?
                                    ":/artwork/artwork/on.svg" :
                                    ":/artwork/artwork/off.svg");

    m_tableView->setImageWidths(width);

    for(int i = 0; i < m_inputTableViewModel->columnCount(); i++)
    {
        QModelIndex index = m_inputTableViewModel->index(row, i);
        m_tableView->indexWidget(index)->setEnabled(isOn);
    }
    QResizeEvent firstEntryFailsRedraw = QResizeEvent(m_tableView->size(), m_tableView->size());
    m_tableView->resizeEvent(&firstEntryFailsRedraw);
}

void InputDialog::redrawTable()
{
    for(int i = 0; i < m_inputList->count(); i++)
    {
        redrawRow(i);
    }
    m_inputTableViewModel->layoutChanged();
}

void InputDialog::slotShowContextMenu(const QPoint& pos)
{
    auto globalPos = m_tableView->mapToGlobal(pos);
    QMenu contextMenu;
    contextMenu.addAction("Delete", this, SLOT(slotDeleteInput()));
    contextMenu.addAction("Source type", this, SLOT(slotChangeSource()));
    contextMenu.addAction("Edit source (fixit)", this, SLOT(slotEditSource()));
    contextMenu.addAction("Processor type", this, SLOT(slotChangeProcessor()));
    contextMenu.addAction("Edit processor (fixit)", this, SLOT(slotEditProcessor()));
    contextMenu.exec(globalPos);
}

QModelIndex InputDialog::getSelectedModelIndex(int column) const
{
    auto rows = m_tableView->selectionModel()->selectedRows();
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
        GetDialog getDialog(m_antiLog);
        sourceEntry->accept(&getDialog);
        break;
    }
    case Column::Processor:
    {
        auto* sourceEntry = m_inputList->getSourceAndProcessor(index.row())->getProcessorEntry();
        GetDialog getDialog(m_antiLog);
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
    getAllSourceTypes(sources);
    InputItemList processors;
    getAllProcessorTypes(processors);

    auto newInputDialog = new NewInputDialog(sources, processors, this);

    if (newInputDialog->exec() == QDialog::Accepted)
    {
        int sourceRow = newInputDialog->currentSourceIndex();
        int processorRow = newInputDialog->currentProcessorIndex();

        auto sap = new SourceAndProcessor(sources.takeAt(sourceRow), processors.takeAt(processorRow));
        m_inputTableViewModel->append(sap);
        m_tableView->selectRow(m_inputTableViewModel->rowCount() - 1);

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

