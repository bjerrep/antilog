#include "globalcolumn.h"
#include "statics.h"
#include "options.h"


GlobalColumn::GlobalColumn(const ColumnType& columnType,
                           const QString& name,
                           const QString& htmlTableDataStart,
                           const QString& htmlTableDataEnd,
                           int length)
    : m_columnType(columnType),
      m_name(name),
      m_htmlTableDataStart(htmlTableDataStart),
      m_htmlTableDataEnd(htmlTableDataEnd),
      m_length(length)
{
}

GlobalColumn::GlobalColumn(const QJsonObject& json)
{
    m_columnType = staticGetTypeFromString(json["type"].toString());
    m_name = json["name"].toString();
    m_htmlTableDataStart = json["tabledatastart"].toString();
    m_htmlTableDataEnd = json["tabledataend"].toString();
    m_enabled = json["enabled"].toBool();
    m_length = json["length"].toInt();
}

void GlobalColumn::save(QJsonObject& json) const
{
    json["type"] = getTypeAsString();
    json["name"] = m_name;
    json["tabledatastart"] = m_htmlTableDataStart;
    json["tabledataend"] = m_htmlTableDataEnd;
    json["enabled"] = m_enabled;
    json["length"] = m_length;
}

QString GlobalColumn::getHtmlTableDataStart() const
{
    int pixelWidth = Statics::instOptions()->logFontWidth(m_length);
    return m_length ? m_htmlTableDataStart.arg(pixelWidth) : m_htmlTableDataStart;
}

QString GlobalColumn::getHtmlTableDataEnd() const
{
    return m_htmlTableDataEnd;
}

GlobalColumn::ColumnType GlobalColumn::getType() const
{
    return m_columnType;
}

int GlobalColumn::getLength() const
{
    return m_length;
}

QString GlobalColumn::staticGetTypeAsString(GlobalColumn::ColumnType columnType)
{
    return QVariant::fromValue(columnType).toString();
}

GlobalColumn::ColumnType GlobalColumn::staticGetTypeFromString(const QString &columnTypeName)
{
    return static_cast<ColumnType>(Statics::metaIndex(staticMetaObject, "ColumnTypeProperty", columnTypeName));
}

void GlobalColumn::setName(const QString &name)
{
    m_name = name;
}

void GlobalColumn::setTypeFromString(const QString &typeString)
{
    m_columnType = staticGetTypeFromString(typeString);
}

QString GlobalColumn::getTypeAsString() const
{
    return QVariant::fromValue(m_columnType).toString();
}

QString GlobalColumn::getName() const
{
    return m_name;
}

bool GlobalColumn::isEnabled() const
{
    return m_enabled;
}
