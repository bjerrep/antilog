#pragma once

#include "inputs/inputbases.h"

#include <QObject>
#include <QList>
#include <QJsonObject>

class AntiLog;
class SourceAndProcessor;
class InputItemBase;

/// Container for the SourceAndProcessor objects made with the InputDialog.
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
    void appendAndConnect(SourceAndProcessor* input);
    void removeAt(int index);
    int findFormatSchemeUsageCount(const QString& schemeName) const;
    SourceAndProcessor* getSourceAndProcessor(int index);
    SourceAndProcessor* getSourceAndProcessor(int index) const;
    InputItemVector getAllSourcesAndProcessors() const;

private:
    QList<SourceAndProcessor*> m_sourceAndProcessorList;
    AntiLog* m_app;
};


