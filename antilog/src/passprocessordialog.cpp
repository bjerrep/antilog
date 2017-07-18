#include "passprocessordialog.h"
#include "ui_passprocessordialog.h"
#include "statics.h"
#include "formatdialog.h"
#include "inputprocessors.h"
#include "formatschememodel.h"

PassProcessorDialog::PassProcessorDialog(PassProcessor* passProcessor, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::PassProcessorDialog),
    m_passProcessor(passProcessor)
{
    ui->setupUi(this);

    ui->iconLayout->addWidget(Statics::widgetIcon(Statics::PassProcessorResource));
    ui->name->setText(passProcessor->getName());

    QString scheme = passProcessor->getScheme()->name();
    ui->comboBoxFormat->addItems(Statics::formatSchemeModel->getSchemeNames());
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
