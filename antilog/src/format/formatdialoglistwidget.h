#pragma once

#include <QListWidget>

class QDragMoveEvent;
class FormatScheme;

class FormatDialogListWidget : public QListWidget
{
    Q_OBJECT

public:
    FormatDialogListWidget(QWidget* parent);
    void setFormatScheme(FormatScheme* formatScheme);

signals:
    void signalDrag();

private:
    void dragMoveEvent(QDragMoveEvent* event);

private slots:
    void slotContentChanged(QListWidgetItem *item);

private:
    FormatScheme* m_formatScheme = nullptr;
};
