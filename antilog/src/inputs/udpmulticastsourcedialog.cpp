#include "udpmulticastsourcedialog.h"
#include "ui_udpmulticastsourcedialog.h"
#include "statics.h"
#include "inputsources.h"
#include "antilog.h"

#include <QFileDialog>
#include <QLineEdit>


UdpMulticastSourceDialog::UdpMulticastSourceDialog(UDPMulticastSource* udpSource, AntiLog* parent) :
    QDialog(parent),
    ui(new Ui::UdpMulticastSourceDialog),
    m_udpSource(udpSource)
{
    ui->setupUi(this);
    ui->name->setText(udpSource->getName());
    ui->iconLayout->addWidget(Statics::widgetIcon(Statics::UDPMulticastSourceResource));
    ui->address->setText(m_udpSource->getAddress());
    ui->port->setText(QString::number(m_udpSource->getPort()));

    setModal(true);

    exec();
}

UdpMulticastSourceDialog::~UdpMulticastSourceDialog()
{
    delete ui;
}

void UdpMulticastSourceDialog::on_fileSelectButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    dialog.setModal(true);
    dialog.exec();
}

void UdpMulticastSourceDialog::on_pushButtonOk_clicked()
{
    close();
    setResult(QDialog::Accepted);
}

void UdpMulticastSourceDialog::on_name_textChanged(const QString &arg1)
{
    m_udpSource->setName(arg1);
}

void UdpMulticastSourceDialog::on_address_textChanged(const QString &arg1)
{
    m_udpSource->setAddress(arg1);
}

void UdpMulticastSourceDialog::on_port_textChanged(const QString &arg1)
{
    m_udpSource->setPort(arg1.toInt());
}
