#pragma once

#include <QObject>
#include <QJsonObject>
#include <QVector>

class FormatScheme;

using FormatSchemeList = QVector<FormatScheme*>;


/// Container for available FormatSchemes
///
class FormatSchemeModel
{
public:
    FormatSchemeModel(const QJsonObject& json);
    ~FormatSchemeModel();

    void save(QJsonObject& json) const;
    FormatScheme* getFormatScheme(const QString& name);
    const FormatSchemeList& getFormatSchemes() const;
    QStringList getSchemeNames();
    int getSchemeNameIndex(const QString& name);
    void addScheme(const QString& name);
    void deleteScheme(const QString& name);

public slots:
    void slotNewLogFont();

private:
    void load(const QJsonObject& json);

private:
    FormatSchemeList m_formatSchemeList;
};
