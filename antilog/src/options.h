#pragma once

#include "statics.h"

#include "globalcolumnconfig.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QFont>
#include <QFontMetrics>

class SchemeColumnModel;

class Options : public QObject
{
    Q_OBJECT

public:
    Options(const QJsonObject& json);
    ~Options();

    void save(QJsonObject& json) const;
    void load(const QJsonObject& json);

    void setLogFont(const QString& family, int point);
    QFontMetrics logFontMetrics() const;
    int logFontWidth(int length) const;
    int getSourceStringMaxWidth(int size);

signals:
    void signalInvalidated();

public:
    const int MAX_WIDTH = 17;
    int m_maxRowsInLogModel = 100000;
    int m_numberOfBytesToTail = 100000;
    QString m_logThreshold = Statics::LogSeverityFilterOff;
    QFont m_logFont;
    QFont m_appFont;
    int m_logFontHeight;
    int m_logViewSpacing = 2;
    QString m_style;
    int m_sourceDataWidth = 0;
    bool m_showSource = false;
};
