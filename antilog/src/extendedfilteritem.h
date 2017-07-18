#pragma once

#include <QObject>
#include <QVector>

class ExtendedFilterItem;
class ExtendedWidget;

using ExtendedFilterList = QVector<ExtendedFilterItem*>;
Q_DECLARE_METATYPE(ExtendedFilterList)

struct ExtendedFilterItem
{
    Q_GADGET

public:
    ExtendedFilterItem(const QJsonObject& json, ExtendedFilterItem* parent);
    ExtendedFilterItem(QString getName, Qt::CheckState getEnableState, const QString& getSeverity, ExtendedFilterItem* parent);
    void save(QJsonObject& json) const;

    QString getName() const { return m_name; }
    Qt::CheckState getEnableState() const { return m_enableState; }
    const ExtendedFilterList& getModules() const { return m_modules; }
    ExtendedFilterItem* getParent() const { return m_parent; }
    QString getSeverity() const { return m_severity; }
    ExtendedWidget* getWidget() const { return m_widget; }

    void addModule(ExtendedFilterItem* filterItem);
    bool isParent() const;
    bool isViewExpanded() const;
    void setViewExpanded(bool expanded);
    void setEnableState(Qt::CheckState state);
    void setSeverity(QString getSeverity);
    void setWidget(ExtendedWidget* getWidget);

private:
    QString m_severity;
    QString m_name;
    Qt::CheckState m_enableState = Qt::Unchecked;
    bool m_isValid = false;
    bool m_viewExpanded = true;
    ExtendedFilterList m_modules;
    ExtendedWidget* m_widget = nullptr;
    ExtendedFilterItem* m_parent = nullptr;

    Q_PROPERTY(Qt::CheckState StateProperty MEMBER m_enableState)
};
