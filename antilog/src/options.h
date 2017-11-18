#pragma once

#include "statics.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QFont>
#include <QFontMetrics>

class ColumnDefinitions;

struct Options
{
    Options(const QJsonObject& json);
    ~Options();

    void save(QJsonObject& json) const;
    void load(const QJsonObject& json);

    void setLogFont(const QString& family, int point);
    QFontMetrics logFontMetrics() const;
    int logFontWidth(int length) const;

    ColumnDefinitions* getColumnDefinitions() const;

    int m_maxRowsInLogModel = 100000;
    int m_numberOfLinesToLoad = 10000;
    QString m_logThreshold = Statics::LogSeverityFilterOff;
    QFont m_logFont;
    QFont m_appFont;
    int m_logFontHeight;
    int m_logViewSpacing = 2;
    QString m_style;
    ColumnDefinitions* m_columnDefinitions;
    bool m_showSource = false;
};
