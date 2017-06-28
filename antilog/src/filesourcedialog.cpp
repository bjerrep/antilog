#include "filesourcedialog.h"
#include "ui_filesourcedialog.h"
#include "filereader.h"

#include <QFileDialog>
#include <QRadioButton>
#include <QFileSystemWatcher>


FileSourceDialog::FileSourceDialog(FileSource& fileSource, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSourceDialog),
    m_fileSource(fileSource)
{
    ui->setupUi(this);

    m_name = fileSource.getName();
    m_filename = fileSource.m_filepath;
    m_tailOnly = fileSource.m_tailOnly;

    ui->iconLayout->addWidget(Statics::widgetIcon(Statics::FileResource));
    ui->name->setText(m_name);
    ui->radioButtonTail->setChecked(m_tailOnly);
    ui->radioButtonRead->setChecked(!m_tailOnly);

    setupNewFilename(m_filename);

    setModal(true);

    if (exec() == QDialog::Accepted)
    {
        bool modified =
                m_name != fileSource.getName() ||
                m_filename != fileSource.m_filepath ||
                m_tailOnly != fileSource.m_tailOnly;

        if (modified)
        {
            fileSource.m_tailOnly = ui->radioButtonTail->isChecked();
            fileSource.setName(m_name);
            fileSource.setFilenameAndConfigure(m_filename);
        }
    }
}

FileSourceDialog::~FileSourceDialog()
{
    delete ui;
}

void FileSourceDialog::slotNewFileData(const QString& data)
{
    qint64 dummy;
    QStringList newline = FileReader::tailFileByLines(data, 1, dummy);
    if (!newline.empty())
    {
        ui->testOutput->setText(newline.first());
    }
    else
    {
        ui->testOutput->setText("");
    }
}

void FileSourceDialog::on_fileSelectButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;

    dialog.setModal(true);

    if (dialog.exec() == QDialog::Accepted)
    {
        fileNames = dialog.selectedFiles();
        if (fileNames.length() == 1)
        {
            setupNewFilename(fileNames[0]);
        }
    }
}

void FileSourceDialog::setupNewFilename(const QString& filename)
{
    ui->filepath->setText(filename);
    ui->testOutput->setText("");
    m_filename = filename;

    if (m_watcher)
    {
        delete m_watcher;
        m_watcher = nullptr;
    }

    if (!QFile(m_filename).exists())
    {
        ui->testOutput->setEnabled(false);
        return;
    }

    ui->testOutput->setEnabled(true);
    m_watcher = new QFileSystemWatcher();
    m_watcher->addPath(filename);
    connect(m_watcher, &QFileSystemWatcher::fileChanged,
            this, &FileSourceDialog::slotNewFileData);
}

void FileSourceDialog::on_pushButtonOk_clicked()
{
    close();
    setResult(QDialog::Accepted);
}

void FileSourceDialog::on_name_textChanged(const QString &arg1)
{
    m_name = arg1;
}

void FileSourceDialog::on_pushButtonCancel_clicked()
{
    close();
    setResult(QDialog::Rejected);
}

void FileSourceDialog::on_radioButtonTail_toggled(bool checked)
{
    m_tailOnly = checked;
}

void FileSourceDialog::on_filepath_editingFinished()
{
    setupNewFilename(ui->filepath->text());
}
