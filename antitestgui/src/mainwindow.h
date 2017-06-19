#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

class QTimer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void save();
    void load();
    QString getConfigFilePath();
    void generateMessages();

public slots:
    void slotTimerHit();

private slots:
    void on_checkBoxExample1_clicked();
    void on_checkBoxHelloWorld_clicked();
    void on_pushButton_clicked();

    void on_spinBoxPeriod_valueChanged(int arg1);

    void on_checkBoxSpdLog_clicked();

    void on_checkBoxAppendSerial_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QTimer* m_timer;
    QString m_filename;
    QStringList m_messages;
    int m_index = 0;
    bool m_appendSerial = false;
};

#endif // MAINWINDOW_H
