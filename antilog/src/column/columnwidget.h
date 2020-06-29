#pragma once

#include <QWidget>
#include <QJsonObject>

class GlobalColumn;


namespace Ui {
class ColumnWidget;
}

class ColumnWidget : public QWidget
{
    Q_OBJECT

public:
    ColumnWidget(const QStringList& columnTypes,
                 QWidget *parent);

    ColumnWidget(const GlobalColumn* column,
                 const QStringList& columnTypes,
                 QWidget *parent);
    ~ColumnWidget();

    static int getHeight();
    GlobalColumn* getColumn() const;

signals:
    void signalColumnWidgetDeleted();

private slots:
    void on_pushButton_delete_clicked();

private:
    Ui::ColumnWidget *ui;
    bool m_deleted = false;
    QJsonObject m_json;
};
