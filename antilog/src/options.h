#pragma once

#include "statics.h"

#include <QJsonObject>
#include <QFont>
#include <QFontMetrics>

struct CellName
{
    CellName()
    {
    }

    CellName(QString name, int width = 0)
        : m_name(name),
          m_width(width)
    {
    }

    QString m_name;
    int m_width = 0;
};

struct CellNames
{
    CellNames()
    {
        m_cells.append(CellName(Statics::Date));
        m_cells.append(CellName(Statics::Time));
        m_cells.append(CellName(Statics::Id));
        m_cells.append(CellName(Statics::Level));
        m_cells.append(CellName(Statics::Message));
    }

    QVector<CellName> m_cells;
};

struct Options
{
    Options(const QJsonObject& json);

    void save(QJsonObject& json) const;
    void load(const QJsonObject& json);

    void setLogFont(const QString& family, int point);
    QFontMetrics logFontMetrics() const;
    int logFontWidth(int length) const;

    int m_maxRowsInLogModel = 100000;
    int m_numberOfLinesToLoad = 10000;
    QString m_logThreshold = Statics::logLevelFilterOff;
    QFont m_logFont;
    QFont m_appFont;
    int m_logFontHeight;
    int m_logViewSpacing = 2;
    QString m_style;
    CellNames m_cellNames;
    bool m_showSource = false;
};
