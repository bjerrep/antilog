#include "extendedfilteritem.h"
#include "extendedwidget.h"
#include "statics.h"

#include <QJsonObject>
#include <QJsonArray>

ExtendedFilterItem::ExtendedFilterItem(const QJsonObject& json, ExtendedFilterItem* parent)
{
    m_name = json["name"].toString();
    m_severity = json["severity"].toString();
    m_enableState = static_cast<Qt::CheckState>(Statics::metaIndex(staticMetaObject, "StateProperty", json["state"].toString()));
    m_parent = parent;
}

ExtendedFilterItem::ExtendedFilterItem(QString name, Qt::CheckState enabled, const QString& severity, ExtendedFilterItem* parent)
    : m_name(name),
      m_enableState(enabled),
      m_isValid(true),
      m_severity(severity),
      m_parent(parent)
{
}

void ExtendedFilterItem::save(QJsonObject& json) const
{
    json["name"] = m_name;
    json["severity"] = m_severity;
    json["state"] = QVariant::fromValue(m_enableState).toString();
}

void ExtendedFilterItem::addModule(ExtendedFilterItem* filterItem)
{
    m_modules.append(filterItem);
}

bool ExtendedFilterItem::isParent() const
{
    return !m_parent;
}

bool ExtendedFilterItem::isViewExpanded() const
{
    return m_viewExpanded;
}

void ExtendedFilterItem::setViewExpanded(bool expanded)
{
    m_viewExpanded = expanded;
}

void ExtendedFilterItem::setEnableState(Qt::CheckState state)
{
    m_enableState = state;
    if (m_widget)
    {
        m_widget->setEnableState(state);
    }
}

void ExtendedFilterItem::setSeverity(QString severity)
{
    m_severity = severity;
    if (m_widget)
    {
        m_widget->setSeverity(severity);
    }
}

void ExtendedFilterItem::setWidget(ExtendedWidget* widget)
{
    m_widget = widget;
}
