#pragma once

#include "extendedfilteritem.h"
#include "logentry.h"

#include <QAbstractItemModel>


/// Registers input source names and module names from incomming log entries in a tree.
/// The rootelements are the input sources containing lists of corresponding module ids.
/// This requires a regex style processor to identify where and what the module ids are, log entries
/// without a module id are ignored.
/// Each element stored have a enable flag and a severity associated, these are edited with the
/// ExtendedFilterDialog dialog.
/// The isMatched() method is used to figure out if a logEntry is currently enabled.
class ExtendedFilterModel : public QObject
{
    Q_OBJECT

public:
    ExtendedFilterModel(const QJsonObject& json);
    ~ExtendedFilterModel();
    void save(QJsonObject& json) const;

    void registerLogEntry(LogEntryPtr logEntry);
    bool isMatched(LogEntryPtr logEntry) const;
    void enableAll(Qt::CheckState enable);
    ExtendedFilterItem* sourceAt(int index);
    ExtendedFilterItem* findModule(ExtendedFilterItem* toFind) const;
    QString getDefaultSeverity() const;
    void setActive(bool active);
    void clear();

signals:
    void signalNewFilterItems(ExtendedFilterItem* parent, ExtendedFilterItem* child);

public:
    Qt::CheckState m_newSourcesEnabled = Qt::Checked;
    mutable ExtendedFilterList m_sources;
    bool m_filterModelIsActive;
    QString m_defaultSeverity = Statics::logLevelFilterOff;
    bool m_persistSourcesAndModules;
};
