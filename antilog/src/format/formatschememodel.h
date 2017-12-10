#pragma once

#include <QObject>
#include <QJsonObject>
#include <QVector>

class FormatScheme;
class GlobalColumnConfig;

using FormatSchemeList = QVector<FormatScheme*>;


/// Container for available FormatSchemes
///
class FormatSchemes
{
public:
    FormatSchemes(const QJsonObject& json, GlobalColumnConfig *columnnLibrary);
    ~FormatSchemes();

    void save(QJsonObject& json) const;
    FormatScheme* getFormatScheme(const QString& name);
    const FormatSchemeList& getFormatSchemes() const;
    QStringList getSchemeNames();
    int getSchemeNameIndex(const QString& name);
    void addScheme(const QString& name);
    void deleteScheme(const QString& name);
    GlobalColumnConfig* getGlobalColumns();

private:
    void load(const QJsonObject& json);

private:
    FormatSchemeList m_formatSchemeList;
    GlobalColumnConfig* m_columnLibrary;
};
