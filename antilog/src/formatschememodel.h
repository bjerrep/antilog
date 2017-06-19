#pragma once

#include "formatscheme.h"
#include "logentryformatter.h"

#include <QJsonObject>
#include <QVector>

using FormatSchemeList = QVector<FormatScheme*>;


/// Container for available FormatSchemes
///
class FormatSchemeModel
{
public:
    FormatSchemeModel(const QJsonObject& json);

    void save(QJsonObject& json) const;
    FormatScheme* getFormatScheme(const QString& name);
    const FormatSchemeList& getFormatSchemes() const;
    void constructTestSchemes();
    QStringList getSchemeNames();
    int getSchemeNameIndex(const QString& name);
    void addScheme(const QString& name);
    void deleteScheme(const QString& name);

public slots:
    void slotNewLogFont();

private:
    void load(const QJsonObject& json);

private:
    FormatSchemeList m_entries;
};
