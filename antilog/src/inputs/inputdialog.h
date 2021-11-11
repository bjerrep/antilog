#pragma once

#include <QDialog>
#include <QTableView>

namespace Ui { class inputDialog; }
class InputList;
class InputTableViewModel;
class AntiLog;
class InputItemBase;



class InputTableView : public QTableView
{
public:
    InputTableView(QWidget* parent);
};


class InputDialog : public QDialog
{
    Q_OBJECT

    enum Column
    {
        Source,
        Arrow,
        Processor,
        Enable
    };

public:
    explicit InputDialog(InputList* inputList, AntiLog* ulw);
    ~InputDialog();

    int insertInputItemWidget(int row, int column, InputItemBase* inputItem);
    int addImageToTable(int row, int column, QString imageResource);
    void redrawRow(int rowIndex);
    void redrawTable();
    QModelIndex getSelectedModelIndex(int column) const;

private slots:
    void slotShowContextMenu(const QPoint& pos);
    void slotDeleteInput();
    void slotChangeSource();
    void slotChangeProcessor();
    void slotEditSource();
    void slotEditProcessor();

    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pushButtonNewInput_clicked();
    void on_pushButtonClose_clicked();

private:
    Ui::inputDialog* ui;
    InputTableView* m_tableView;
    InputTableViewModel* m_inputTableViewModel;
    InputList* m_inputList;
    AntiLog* m_antiLog;
};

