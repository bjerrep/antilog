#include "newinputdialog.h"
#include "ui_newinputdialog.h"
#include "statics.h"
#include "sourcevisitors.h"

#include <QListWidgetItem>

NewInputDialog::NewInputDialog(const InputItemList& sources,
                               const InputItemList& processors,
                               QWidget *parent)
    : QDialog(parent),
      ui(new Ui::NewInputDialog),
      m_sources(sources),
      m_processors(processors)
{
    ui->setupUi(this);
    drawDialog();
    setModal(true);
}

NewInputDialog::~NewInputDialog()
{
    delete ui;
}

void NewInputDialog::drawDialog()
{
    ui->processors->setLayoutDirection(Qt::LeftToRight);
    foreach (auto source, m_sources)
    {
        auto getLabel = new GetLabel();
        source->accept(getLabel);
        auto item = new QListWidgetItem();
        item->setSizeHint(getLabel->sizeHint());
        ui->sources->addItem(item);
        ui->sources->setItemWidget(item, getLabel);
    }

    foreach (auto processor, m_processors)
    {
        auto getLabel = new GetLabel();
        processor->accept(getLabel);
        auto item = new QListWidgetItem();
        item->setSizeHint(getLabel->sizeHint());
        ui->processors->addItem(item);
        ui->processors->setItemWidget(item, getLabel);
    }

    ui->processors->setFocusPolicy(Qt::NoFocus);
    ui->sources->setFocusPolicy(Qt::NoFocus);

    ui->sources->setCurrentIndex(ui->sources->indexAt(QPoint(0, 0)));
    ui->processors->setCurrentIndex(ui->processors->indexAt(QPoint(0, 0)));
}

int NewInputDialog::currentSourceIndex() const
{
    return ui->sources->currentIndex().row();
}

int NewInputDialog::currentProcessorIndex() const
{
    return ui->processors->currentIndex().row();
}

void NewInputDialog::on_pushButtonOk_clicked()
{
    close();
    setResult(QDialog::Accepted);
}
