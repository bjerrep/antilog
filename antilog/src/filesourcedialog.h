#pragma once

#include "inputsources.h"

#include <QDialog>

class QFileSystemWatcher;

namespace Ui {
    class FileSourceDialog;
}

class FileSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileSourceDialog(FileSource& fileSource, QWidget *parent);
    ~FileSourceDialog();

private:
    void setupNewFilename(const QString& filename);

private slots:
    void slotNewFileData(const QString& data);

    void on_fileSelectButton_clicked();
    void on_pushButtonOk_clicked();
    void on_name_textChanged(const QString &arg1);
    void on_pushButtonCancel_clicked();
    void on_radioButtonTail_toggled(bool checked);
    void on_filepath_editingFinished();

private:
    Ui::FileSourceDialog *ui;
    const FileSource& m_fileSource;
    QString m_name;
    QString m_filename;
    bool m_tailOnly;
    QFileSystemWatcher* m_watcher = nullptr;
};
