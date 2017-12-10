#pragma once

#include "globalcolumn.h"

#include <QObject>
#include <QVector>
#include <QJsonObject>

class SchemeColumn;

using SchemeColumnVector = QVector<SchemeColumn*>;
using GlobalColumnTypeList = QVector<GlobalColumn::ColumnType>;


class SchemeColumnModel : public QObject
{
    Q_OBJECT

public:
    SchemeColumnModel();
    SchemeColumnModel(const QJsonObject& json);
    ~SchemeColumnModel();

    void load(const QJsonObject& json);
    void save(QJsonObject& json) const;

    int getNofEnabledColumns() const;
    SchemeColumnVector getColumnVector() const;
    QStringList getColumnTypeStringlist() const;
    const GlobalColumnTypeList& getEnabledColumnTypes() const;

    void reset();
    SchemeColumn* findUid(int uid);
    void refreshColumns(SchemeColumnVector& columnList);
    void setActiveCells(const QStringList& names);

    int getLogSeverityIndex() const;
    int getModuleIdIndex() const;

    static const int IndexNotFound = -1;

signals:
    void signalInvalidated();

private:
    void clear();
    void invalidated();

private:
    int findLogSeverityIndex();
    int findModuleIdIndex();

public:
    SchemeColumnVector m_schemeColumnVector;
    GlobalColumnTypeList m_enabledColumnTypeList;
    int m_logSeverityIndex = IndexNotFound;
    int m_moduleIdIndex = IndexNotFound;
};
