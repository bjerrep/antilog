#include "schemecolumn.h"
#include "statics.h"
#include "options.h"
#include "globalcolumnconfig.h"

#include <QJsonArray>
#include <QMetaEnum>


int s_columnUid = 0;


SchemeColumn::SchemeColumn(GlobalColumn::ColumnType cellType, bool enabled)
    : QObject(),
      m_columnType(cellType),
      m_enabled(enabled)
{
}

SchemeColumn::SchemeColumn(const QJsonObject& json)
    : QObject()
{
    load(json);
}

SchemeColumn::SchemeColumn(const SchemeColumn& src) : QObject()
{
    m_columnType = src.m_columnType;
    m_enabled = src.m_enabled;
    m_uid = src.m_uid;
}

void SchemeColumn::load(const QJsonObject &json)
{
    m_columnType = GlobalColumn::staticGetTypeFromString(json["type"].toString());
    m_enabled = json["enabled"].toBool();
}

void SchemeColumn::save(QJsonObject& json) const
{
    json["type"] = getTypeAsString();
    json["enabled"] = m_enabled;
}

GlobalColumn::ColumnType SchemeColumn::getCellType() const
{
    return m_columnType;
}

QString SchemeColumn::getTypeAsString() const
{
    return GlobalColumn::staticGetTypeAsString(m_columnType);
}

bool SchemeColumn::isEnabled() const
{
    return m_enabled;
}

int SchemeColumn::getUid() const
{
    return m_uid;
}

