#include "loglevels.h"

#include <QJsonObject>
#include <QVariant>
#include <QJsonArray>

// ------ CategoryAlias -------

CategoryAlias::CategoryAlias(const QString& category)
    : m_category(category)
{
}

CategoryAlias::CategoryAlias(const QJsonObject& json)
{
    m_category = json["category"].toString();
    QJsonArray array = json["alias"].toArray();
    foreach (auto variant, array.toVariantList())
    {
        m_aliasList << variant.toString();
    }
}

void CategoryAlias::save(QJsonObject& json) const
{
    json["category"] = m_category;
    json["alias"] = QJsonValue::fromVariant(m_aliasList);
}

bool CategoryAlias::contains(const QString& alias) const
{
    return m_aliasList.indexOf(alias) >= 0;
}

// ------ LogLevelCategories -------

LogLevelCategories::LogLevelCategories(const QJsonObject& json)
{
    if (json.empty())
    {
        constructDefaultCategories();
        constructCategoryNames();
        return;
    }

    auto categories = json["categories"].toArray();
    foreach (auto jsonvalue, categories)
    {
        auto obj = jsonvalue.toObject();
        CategoryAlias categoryAlias(obj);
        m_categories.append(categoryAlias);
    }
    constructCategoryNames();
}

void LogLevelCategories::save(QJsonObject& json) const
{
    QJsonArray categories;
    foreach (auto categoryAlias, m_categories)
    {
        QJsonObject obj;
        categoryAlias.save(obj);
        categories.append(obj);
    }

    json["categories"] = categories;
}

void LogLevelCategories::constructCategoryNames()
{
    m_categoryNames.clear();
    foreach (auto threshold, m_categories)
    {
        m_categoryNames << threshold.m_category;
    }
}

QStringList const& LogLevelCategories::getCategoryNames() const
{
    return m_categoryNames;
}

QString LogLevelCategories::getDefaultLoglevel()
{
    return "Standard";
}

int LogLevelCategories::value(const QString& category) const
{
    return m_categoryNames.indexOf(category);
}

QString LogLevelCategories::findLogLevel(const QString& logLevelString) const
{
    const QString& lowerCase = logLevelString.toLower();

    foreach (auto category, m_categories)
    {
        if (category.contains(lowerCase))
        {
            return category.m_category;
        }
    }
    return Statics::logLevelFilterOff;
}

void LogLevelCategories::constructDefaultCategories()
{
    CategoryAlias t0(Statics::logLevelFilterOff);
    m_categories.append(t0);

    CategoryAlias t1("Devel");
    t1.m_aliasList << "verbose" << "ver";
    m_categories.append(t1);

    CategoryAlias t2("Debug");
    t2.m_aliasList << "debug" << "deb";
    m_categories.append(t2);

    CategoryAlias t3("Extra");
    t3.m_aliasList << "info" << "inf";
    m_categories.append(t3);

    CategoryAlias t4("Standard");
    t4.m_aliasList << "release" << "rel";
    m_categories.append(t4);

    CategoryAlias t5("Fails");
    t5.m_aliasList << "warning" << "war" << "error" << "err" << "fatal" << "fat" << "critical";
    m_categories.append(t5);
}
