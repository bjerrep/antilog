#pragma once

#include <QDialog>

class UDPSource;

class AntiLog;

namespace Ui
{
class UdpSourceDialog;
}

class UdpSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UdpSourceDialog(UDPSource* udpSource, AntiLog* parent);
    ~UdpSourceDialog();

private slots:
    void on_fileSelectButton_clicked();
    void on_pushButtonOk_clicked();
    void on_name_textChanged(const QString &arg1);
    void on_address_textChanged(const QString &arg1);
    void on_port_textChanged(const QString &arg1);

private:
    Ui::UdpSourceDialog* ui;
    UDPSource* m_udpSource;
};
