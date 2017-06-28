#pragma once

#include "inputbases.h"

#include <QDialog>

class InputItemBase;

namespace Ui
{
    class NewInputDialog;
}

class NewInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewInputDialog(const InputItemList& sources,
                            const InputItemList& processors,
                            QWidget *parent);
    ~NewInputDialog();

    void drawDialog();
    int currentSourceIndex() const;
    int currentProcessorIndex() const;

private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::NewInputDialog *ui;
    const InputItemList& m_sources;
    const InputItemList& m_processors;
};

