#pragma once

#include "columndefinitions.h"

#include <QWidget>

namespace Ui {
class ColumnWidget;
}

class ColumnWidget : public QWidget
{
    Q_OBJECT

public:
    ColumnWidget(const QStringList& columnTypes,
                 QWidget *parent);
    ColumnWidget(const Column* column,
                 const QStringList& columnTypes,
                 QWidget *parent);
    ~ColumnWidget();

    int getHeight() const;
    Column* getColumn() const;

signals:
    void signalColumnWidgetDeleted();

private slots:
    void on_pushButton_delete_clicked();

private:
    Ui::ColumnWidget *ui;
    bool m_deleted = false;
    int m_uid = -1;
};
