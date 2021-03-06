#include "passprocessordialog.h"
#include "ui_passprocessordialog.h"
#include "statics.h"
#include "inputprocessors.h"
#include "format/formatdialog.h"
#include "format/formatscheme.h"
#include "format/formatschememodel.h"
#include "antilog.h"

PassProcessorDialog::PassProcessorDialog(PassProcessor* passProcessor, AntiLog* parent) :
    QDialog(parent),
    ui(new Ui::PassProcessorDialog),
    m_passProcessor(passProcessor)
{
    ui->setupUi(this);

    ui->iconLayout->addWidget(Statics::widgetIcon(Statics::PassProcessorResource));
    ui->name->setText(passProcessor->getName());

    QString scheme = passProcessor->getScheme()->getName();
    ui->comboBoxFormat->addItems(Statics::instAntiLog()->getFormatSchemeModel()->getSchemeNames());
    ui->comboBoxFormat->setCurrentText(scheme);

    setModal(true);

    exec();
}

PassProcessorDialog::~PassProcessorDialog()
{
    delete ui;
}

void PassProcessorDialog::on_comboBoxFormat_currentTextChanged(const QString &arg1)
{
    m_passProcessor->setSchemeFromName(arg1);
}

void PassProcessorDialog::on_name_textChanged(const QString &arg1)
{
    m_passProcessor->setName(arg1);
}

void PassProcessorDialog::on_pushButtonOk_clicked()
{
    close();
    setResult(QDialog::Accepted);
}

void PassProcessorDialog::on_pushButton_clicked()
{
    auto formatDialog = new FormatDialog(ui->comboBoxFormat->currentText(), this);
    formatDialog->exec();
}
