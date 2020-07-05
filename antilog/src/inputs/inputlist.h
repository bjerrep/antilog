#pragma once

#include "inputs/inputbases.h"

#include <QObject>
#include <QList>
#include <QJsonObject>

class AntiLog;
class Input;
class InputItemBase;

using InputListType = QList<Input*>;

/// Container for the Input objects made with the InputDialog.
/// A source together with a processor is called an 'input'.
/// Owned by the antilog class.
///
class InputList : public QObject
{
    Q_OBJECT

public:
    InputList(AntiLog* antiLog);
    ~InputList();

    void save(QJsonObject& json) const;
    void load(const QJsonObject& json);
    int count() const;
    void appendAndConnect(Input* input);
    void removeAt(int index);
    int findFormatSchemeUsageCount(const QString& schemeName) const;

    Input* getInput(int index);
    Input* getInput(int index) const;
    InputListType getAllInputs();

    InputItemVector getAllItemInputs() const;

private:
    InputListType m_inputList;
    AntiLog* m_app;
};


