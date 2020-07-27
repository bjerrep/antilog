#pragma once

#include <QMainWindow>
#include <QStringList>
#include <QUdpSocket>

class QTimer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void save();
    void load();
    QString getConfigFilePath();
    QString getNextMessage();
    void UDPOpen();
    void UDPClose();
    void UDPMulticastOpen();
    void UDPMulticastClose();

public slots:
    void slotTimerHit();

private slots:
    void on_pushButton_clicked();
    void on_spinBoxPeriod_valueChanged(int arg1);
    void on_checkBoxAppendSerial_clicked(bool checked);
    void on_pushButtonDeleteFile_clicked();
    void on_checkBoxUDP_clicked(bool checked);
    void on_checkBoxMulticast_clicked(bool checked);

private:
    Ui::MainWindow* ui;
    QTimer* m_timer;
    int m_serial = 0;
    bool m_appendSerial = false;
    int m_messageSelector = 0;
    int m_helloWorldIndex = 0;
    int m_example1Index = 0;
    int m_spdLogIndex = 0;
    QStringList m_helloworld;
    QStringList m_example1;
    QStringList m_spdlog;
    QUdpSocket* m_socket = nullptr;
    QUdpSocket* m_multicastSocket = nullptr;
};

