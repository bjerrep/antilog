#pragma once

#include <QDialog>

class UDPMulticastSource;

class AntiLog;

namespace Ui
{
class UdpMulticastSourceDialog;
}

class UdpMulticastSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UdpMulticastSourceDialog(UDPMulticastSource* udpSource, AntiLog* parent);
    ~UdpMulticastSourceDialog();

private slots:
    void on_fileSelectButton_clicked();
    void on_pushButtonOk_clicked();
    void on_name_textChanged(const QString &arg1);
    void on_address_textChanged(const QString &arg1);
    void on_port_textChanged(const QString &arg1);

private:
    Ui::UdpMulticastSourceDialog* ui;
    UDPMulticastSource* m_udpSource;
};
