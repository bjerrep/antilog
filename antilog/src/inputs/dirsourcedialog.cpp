#include "dirsourcedialog.h"
#include "ui_dirsourcedialog.h"
#include "filereader.h"

#include <QFileDialog>

DirSourceDialog::DirSourceDialog(DirSource& dirSource, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DirSourceDialog)
{
    ui->setupUi(this);

    ui->IconLayout->addWidget(Statics::widgetIcon(Statics::DirResource));
    ui->NameLineEdit->setText(dirSource.getName());
    ui->TargetLineEdit->setText(dirSource.m_dir);
    ui->RecurseCheckBox->setChecked(dirSource.m_recursive);
    ui->FileMaskLineEdit->setText(dirSource.m_mask);

    updateUi(dirSource.m_dir);

    setModal(true);
    if (exec() == QDialog::Accepted)
    {
        dirSource.setName(ui->NameLineEdit->text());
        dirSource.setDir(ui->TargetLineEdit->text());
        dirSource.m_recursive = ui->RecurseCheckBox->isChecked();
        dirSource.m_mask = ui->FileMaskLineEdit->text();
        dirSource.relaunchFileReaderProcess();
    }
}

DirSourceDialog::~DirSourceDialog()
{
    delete ui;
}

void DirSourceDialog::updateUi(const QString& dir)
{
    ui->TargetLineEdit->setText(dir);
    QStringList files;
    QStringList directories;
    int noAccess = FileReader::glob(dir, files, directories,
                                    ui->FileMaskLineEdit->text(),
                                    ui->RecurseCheckBox->isChecked());
    ui->FileListWidget->clear();
    ui->FileListWidget->addItems(files);
    QString info = QString("%1 file(s)").arg(files.count());
    if (noAccess > 0)
    {
        info += QString(", %1 file(s) can't be read by this user").arg(noAccess);
    }
    else if (noAccess == -1)
    {
        info = "not a directory";
    }
    ui->FileCountLabel->setText(info);
}

void DirSourceDialog::on_DirSelectPushButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory("/var/log");
    QStringList fileNames;

    dialog.setModal(true);

    if (dialog.exec() == QDialog::Accepted)
    {
        fileNames = dialog.selectedFiles();
        updateUi(fileNames.at(0));
    }
}

void DirSourceDialog::on_OkPushButton_clicked()
{
    close();
    setResult(QDialog::Accepted);
}

void DirSourceDialog::on_CancelPushButton_clicked()
{
    close();
    setResult(QDialog::Rejected);
}

void DirSourceDialog::on_RecurseCheckBox_clicked(bool /*checked*/)
{
    updateUi(ui->TargetLineEdit->text());
}

void DirSourceDialog::on_FileMaskLineEdit_editingFinished()
{
    updateUi(ui->TargetLineEdit->text());
}

void DirSourceDialog::on_TargetLineEdit_returnPressed()
{
    blockSignals(true);
    updateUi(ui->TargetLineEdit->text());
    blockSignals(false);
}

