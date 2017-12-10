#include "schemecolumnmodel.h"
#include "schemecolumn.h"

#include <QJsonArray>
#include <QMetaEnum>

SchemeColumnModel::SchemeColumnModel()
{
    reset();
}

SchemeColumnModel::SchemeColumnModel(const QJsonObject& json)
{
    load(json);
}

SchemeColumnModel::~SchemeColumnModel()
{
    clear();
}

void SchemeColumnModel::load(const QJsonObject& json)
{
    if (json.empty() || json["columntypes"].toArray().empty())
    {
        reset();
        return;
    }
    auto identifiers = json["columntypes"].toArray();
    foreach (auto cellIdentifier, identifiers)
    {
        m_schemeColumnVector.append(new SchemeColumn(cellIdentifier.toObject()));
    }
    invalidated();
}

void SchemeColumnModel::save(QJsonObject& json) const
{
    QJsonArray jsonArray;
    foreach (auto column, m_schemeColumnVector)
    {
        QJsonObject obj;
        column->save(obj);
        jsonArray.append(obj);
    }
    json["columntypes"] = jsonArray;
}

int SchemeColumnModel::getNofEnabledColumns() const
{
    int count = 0;
    foreach (auto col, m_schemeColumnVector)
    {
        if (col->isEnabled())
            count++;
    }
    return count;
}

SchemeColumnVector SchemeColumnModel::getColumnVector() const
{
    return m_schemeColumnVector;
}

QStringList SchemeColumnModel::getColumnTypeStringlist() const
{
    QStringList columnTypes;

    for (auto& schemeColumn : m_schemeColumnVector)
    {
        if (schemeColumn->isEnabled())
        {
            columnTypes.append(schemeColumn->getTypeAsString());
        }
    }
    return columnTypes;
}

const GlobalColumnTypeList &SchemeColumnModel::getEnabledColumnTypes() const
{
    return m_enabledColumnTypeList;
}

void SchemeColumnModel::clear()
{
    qDeleteAll(m_schemeColumnVector);
    m_schemeColumnVector.clear();
}

void SchemeColumnModel::invalidated()
{
    m_enabledColumnTypeList.clear();
    for (auto& col: m_schemeColumnVector)
    {
        if (col->isEnabled())
        {
            m_enabledColumnTypeList.append(col->getCellType());
        }
    }
    findLogSeverityIndex();
    findModuleIdIndex();
}

void SchemeColumnModel::reset()
{
    clear();
    auto list = Statics::instColumnLibrary()->getTableCellFormatMap();
    for (auto columnType: list)
    {
        m_schemeColumnVector.append(new SchemeColumn(columnType->getType(), true));
    }
    invalidated();
}

SchemeColumn* SchemeColumnModel::findUid(int uid)
{
    foreach (auto column, m_schemeColumnVector)
    {
        if (column->getUid() == uid)
        {
            return column;
        }
    }
    return nullptr;
}

void SchemeColumnModel::refreshColumns(SchemeColumnVector &columnList)
{
    bool modified = false;

    foreach (auto column, columnList)
    {
        if (SchemeColumn* columnFound = findUid(column->getUid()))
        {
            if (!columnFound)
            {
                m_schemeColumnVector.append(column);
                modified = true;
            }
        }
    }
    if (modified)
    {
        invalidated();
        emit signalInvalidated();
    }
}

int SchemeColumnModel::findLogSeverityIndex()
{
    m_logSeverityIndex = 0;
    foreach (auto column, m_schemeColumnVector)
    {
        if (column->m_enabled && column->getCellType() == GlobalColumn::SEVERITY)
        {
            return m_logSeverityIndex;
        }
        ++m_logSeverityIndex;
    }
    m_logSeverityIndex = IndexNotFound;
    return m_logSeverityIndex;
}

int SchemeColumnModel::findModuleIdIndex()
{
    m_moduleIdIndex = 0;
    foreach (auto column, m_schemeColumnVector)
    {
        if (column->m_enabled && column->getCellType() == GlobalColumn::MODULEID)
        {
            return m_moduleIdIndex;
        }
        ++m_moduleIdIndex;
    }
    m_moduleIdIndex = IndexNotFound;
    return m_moduleIdIndex;
}

void SchemeColumnModel::setActiveCells(const QStringList& names)
{
    // trying to do two things, update the enabled flags and reorder to match the
    // order in names
    SchemeColumnVector orderedList;

    foreach (auto name, names)
    {
        foreach (auto schemeColumn, m_schemeColumnVector)
        {
            QString _name = Statics::instColumnLibrary()->getName(schemeColumn->getCellType());
            if (_name == name)
            {
                schemeColumn->m_enabled = true;
                orderedList.append(schemeColumn);
                continue;
            }
        }
    }

    foreach (auto found, orderedList)
    {
        m_schemeColumnVector.removeAll(found);
    }

    foreach (auto remaining, m_schemeColumnVector)
    {
        remaining->m_enabled = false;
        orderedList.append(remaining);
    }

    m_schemeColumnVector = orderedList;

    invalidated();
}

int SchemeColumnModel::getLogSeverityIndex() const
{
    return m_logSeverityIndex;
}

int SchemeColumnModel::getModuleIdIndex() const
{
    return m_moduleIdIndex;
}
