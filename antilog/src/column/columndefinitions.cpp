#include "columndefinitions.h"
#include "statics.h"

#include <QJsonArray>
#include <QMetaEnum>

int s_columnUid = 0;


Column::Column()
    : QObject()
{
}

Column::Column(ColumnType cellType, const QString& name, bool enabled)
    : QObject(),
      m_columnType(cellType),
      m_name(name),
      m_enabled(enabled)
{
}

Column::Column(const QString &columnTypeName, const QString &name, bool enabled, int uid)
    : QObject(),
      m_columnType(static_cast<ColumnType>(Statics::metaIndex(staticMetaObject, "ColumnTypeProperty", columnTypeName))),
      m_name(name),
      m_enabled(enabled)
{
    if (uid >= 0)
    {
        m_uid = uid;
    }
}

Column::Column(const QJsonObject& json)
    : QObject()
{
    load(json);
}

Column::Column(const Column& src) : QObject()
{
    m_columnType = src.m_columnType;
    m_name = src.m_name;
    m_enabled = src.m_enabled;
    m_uid = src.m_uid;
    m_deleted = src.m_deleted;
}

void Column::load(const QJsonObject &json)
{
    QString columnTypeName = json["type"].toString();
    m_columnType = static_cast<ColumnType>(Statics::metaIndex(staticMetaObject, "ColumnTypeProperty", columnTypeName));
    m_name = json["name"].toString();
    m_enabled = json["enabled"].toBool();
}

void Column::save(QJsonObject& json) const
{
    json["type"] = getCellTypeAsString();
    json["name"] = m_name;
    json["enabled"] = m_enabled;
}

Column::ColumnType Column::getCellType() const
{
    return m_columnType;
}

QString Column::getCellTypeAsString() const
{
    return QVariant::fromValue(m_columnType).toString();
}

QString Column::getCellTypeAsString(ColumnType columnType)
{
    return QVariant::fromValue(columnType).toString();
}

Column::ColumnType Column::getCellTypeFromString(const QString &columnTypeName)
{
    return static_cast<ColumnType>(Statics::metaIndex(staticMetaObject, "ColumnTypeProperty", columnTypeName));
}

QString Column::getName() const
{
    return m_name;
}

bool Column::isEnabled() const
{
    return m_enabled;
}

int Column::getUid() const
{
    return m_uid;
}

void Column::setDeleted()
{
    m_deleted = true;
}




ColumnDefinitions::ColumnDefinitions(const QJsonObject& json)
{
    if (json.empty() || json["columntypes"].toArray().empty())
    {
        reset();
        return;
    }
    auto identifiers = json["columntypes"].toArray();
    foreach (auto cellIdentifier, identifiers)
    {
        m_columnList.append(new Column(cellIdentifier.toObject()));
    }
}

ColumnDefinitions::~ColumnDefinitions()
{
    clear();
}

void ColumnDefinitions::save(QJsonObject& json)
{
    QJsonArray jsonArray;
    foreach (auto column, m_columnList)
    {
        QJsonObject obj;
        column->save(obj);
        jsonArray.append(obj);
    }
    json["columntypes"] = jsonArray;
}

ColumnList ColumnDefinitions::getColumnTypeList() const
{
    return m_columnList;
}

QStringList ColumnDefinitions::getColumnTypeStringlist() const
{
    QStringList columnTypes;

    int index = Column::staticMetaObject.indexOfEnumerator("ColumnType");
    auto meta = Column::staticMetaObject.enumerator(index);

    for (int i = 0; i < meta.keyCount(); i++)
    {
        columnTypes << meta.key(i);
    }
    return columnTypes;
}

void ColumnDefinitions::clear()
{
    qDeleteAll(m_columnList);
    m_columnList.clear();
}

void ColumnDefinitions::reset()
{
    clear();
    m_columnList.append(new Column(Column::DATE, Statics::DefaultDateName, true));
    m_columnList.append(new Column(Column::TIME, Statics::DefaultTimeName, true));
    m_columnList.append(new Column(Column::MODULEID, Statics::DefaultModuleIdName, true));
    m_columnList.append(new Column(Column::SEVERITY, Statics::DefaultSeverityName, true));
    m_columnList.append(new Column(Column::TEXT, Statics::DefaultTextName, true));
    //Column::ANY is not included here
}

Column* ColumnDefinitions::findUid(int uid)
{
    foreach (auto column, m_columnList)
    {
        if (column->getUid() == uid)
        {
            return column;
        }
    }
    return nullptr;
}

void ColumnDefinitions::refreshColumns(ColumnList &columnList)
{
    bool invalidated = false;

    foreach (auto column, columnList)
    {
        if (Column* columnFound = findUid(column->getUid()))
        {
            if (!columnFound)
            {
                m_columnList.append(column);
                invalidated = true;
            }
            else if (column->m_deleted)
            {
                m_columnList.removeOne(columnFound);
                invalidated = true;
            }
        }
    }
    if (invalidated)
    {
        emit signalInvalidated();
    }
}
