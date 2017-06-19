#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QCheckBox>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QLineEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QListWidget>
#include <QStringList>
#include <QtDebug>
#include <QStandardPaths>
#include <QDir>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    load();

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()) , this, SLOT(slotTimerHit()));
    ui->spinBoxPeriod->setValue(1000);
}

MainWindow::~MainWindow()
{
    save();
    delete ui;
}

void MainWindow::save()
{
    QJsonObject json;
    json["filename"] = ui->lineEditFile->text();
    json["tofile"] = ui->checkBoxFile->isChecked();
    json["tostdout"] = ui->checkBoxStdout->isChecked();

    QFile file(getConfigFilePath());
    if (file.open(QIODevice::WriteOnly))
    {
        QJsonDocument doc(json);
        file.write(doc.toJson());
    }
}

void MainWindow::load()
{
    QFile file(getConfigFilePath());
    if (!file.open(QIODevice::ReadOnly))
    {
        ui->lineEditFile->setText("filename_to_write.txt");
        return;
    }

    QJsonDocument doc(QJsonDocument::fromJson(file.readAll()));
    QJsonObject json = doc.object();

    QString filename = json["filename"].toString();
    ui->lineEditFile->setText(filename);
    ui->checkBoxFile->setChecked(json["tofile"].toBool());
    ui->checkBoxStdout->setChecked(json["tostdout"].toBool());
}

void MainWindow::slotTimerHit()
{
    if (!m_messages.size())
        return;

    QString message = m_messages[m_index % m_messages.size()];
    if (m_appendSerial)
        message += " (" + QString::number(m_index) + ")";
    ++m_index;

    if (ui->checkBoxFile->isChecked())
    {
        QFile file(ui->lineEditFile->text());
        if (file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream stream( &file );
            stream << message << '\n';
            file.close();
        }
    }

    if (ui->checkBoxFile->isChecked())
    {
        ui->listWidgetPreview->addItem(message);
        ui->listWidgetPreview->scrollToBottom();
    }
}

void MainWindow::generateMessages()
{
    QStringList helloworld;
    helloworld
            << "hello world"
            << "who are you";

    QStringList example1;
    example1
            << "2017-01-01 10:20:30:000 ver [fuelcell] on fire"
            << "2017-01-01 10:20:31:000 deb [fuelcell] sold"
            << "2017-01-02 10:20:30:000 inf [fuelcell] left the building"
            << "2017-01-03 10:20:30:000 rel [fuelcell] captured the flag"
            << "2017-01-03 10:20:30:000 war [fuelcell] que?"
            << "2017-01-03 10:20:30:000 err [fuelcell] oups"
            << "2017-01-03 10:20:30:000 fat [fuelcell] arrghhh";

    QStringList spdlog;
    spdlog
            << "[2017-06-09 00:09:46.850] [console] [info] Welcome to spdlog!"
            << "[2017-06-09 00:09:46.850] [console] [error] Some error message with arg1.."
            << "[2017-06-09 00:09:46.850] [console] [warning] Easy padding in numbers like 00000012"
            << "[2017-06-09 00:09:46.850] [console] [critical] Support for int: 42;  hex: 2a;  oct: 52; bin: 101010"
            << "[2017-06-09 00:09:46.850] [console] [info] Support for floats 1.23"
            << "[2017-06-09 00:09:46.850] [console] [info] Positional args are supported too.."
            << "[2017-06-09 00:09:46.850] [console] [info] left aligned"
            << "[2017-06-09 00:09:46.850] [console] [info] loggers can be retrieved from a global registry using the spdlog::get(logger_name) function";

    m_messages.clear();

    for (int i = 0; i <= 100; i++)
    {
        if (ui->checkBoxHelloWorld->isChecked())
            m_messages.append(helloworld[i % helloworld.size()]);
        if (ui->checkBoxExample1->isChecked())
            m_messages.append(example1[i % example1.size()]);
        if (ui->checkBoxSpdLog->isChecked())
            m_messages.append(spdlog[i % spdlog.size()]);
    }
}

QString MainWindow::getConfigFilePath()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(path);
    return QDir(path).filePath("configtestui.json");
}

void MainWindow::on_checkBoxExample1_clicked()
{
    generateMessages();
}

void MainWindow::on_checkBoxHelloWorld_clicked()
{
    generateMessages();
}

void MainWindow::on_pushButton_clicked()
{
    close();
}

void MainWindow::on_spinBoxPeriod_valueChanged(int arg1)
{
    m_timer->setInterval(arg1);
    m_timer->start();
}

void MainWindow::on_checkBoxSpdLog_clicked()
{
    generateMessages();
}

void MainWindow::on_checkBoxAppendSerial_clicked(bool checked)
{
    m_appendSerial = checked;
}
