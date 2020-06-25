#include "udpsourcedialog.h"
#include "ui_udpsourcedialog.h"
#include "statics.h"
#include "inputsources.h"
#include "antilog.h"

#include <QFileDialog>
#include <QLineEdit>


UdpSourceDialog::UdpSourceDialog(UDPSource* udpSource, AntiLog* parent) :
    QDialog(parent),
    ui(new Ui::UdpSourceDialog),
    m_udpSource(udpSource)
{
    ui->setupUi(this);
    ui->name->setText(udpSource->getName());
    ui->iconLayout->addWidget(Statics::widgetIcon(Statics::UDPSourceResource));
    ui->address->setText(m_udpSource->getAddress());
    ui->port->setText(QString::number(m_udpSource->getPort()));

    setModal(true);

    exec();
}

UdpSourceDialog::~UdpSourceDialog()
{
    delete ui;
}

void UdpSourceDialog::on_fileSelectButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    dialog.setModal(true);
    dialog.exec();
}

void UdpSourceDialog::on_pushButtonOk_clicked()
{
    close();
    setResult(QDialog::Accepted);
}

void UdpSourceDialog::on_name_textChanged(const QString &arg1)
{
    m_udpSource->setName(arg1);
}

void UdpSourceDialog::on_address_textChanged(const QString &arg1)
{
    m_udpSource->setAddress(arg1);
}

void UdpSourceDialog::on_port_textChanged(const QString &arg1)
{
    m_udpSource->setPort(arg1.toInt());
}
