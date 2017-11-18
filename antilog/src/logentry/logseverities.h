#pragma once
#include "statics.h"

#include <QStringList>
#include <QJsonObject>

class CategoryAlias
{
    CategoryAlias(const QString& category);
    CategoryAlias(const QJsonObject& json);

    void save(QJsonObject& json) const;
    bool contains(const QString& alias) const;

    QString m_category;
    QStringList m_aliasList;

    friend class LogSeverityCategories;
};

/// Incomming log severity strings are mapped to a fixed set of 'categories'
/// which have a number of alias definitions. So e.g "err" and "critical" will
/// both belong into the fixed category called "Fails".
///
class LogSeverityCategories
{
public:
    LogSeverityCategories(const QJsonObject& json);

    void save(QJsonObject& json) const;
    QStringList const& getCategoryNames() const;
    static QString getDefaultSeverity();
    int value(const QString& category) const;
    QString findLogSeverity(const QString& logSeverityString) const;

private:
    void constructDefaultCategories();
    void constructCategoryNames();

private:
    QList<CategoryAlias> m_categories;
    QStringList m_categoryNames;
};
