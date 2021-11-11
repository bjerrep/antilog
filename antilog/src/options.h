#pragma once

#include "statics.h"

#include "column/globalcolumnconfig.h"

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

    void setAppFont(QFont font);
    QFont& getAppFont();
    void setLogFont(QFont font);
    QFont& getLogFont();

    QFontMetrics logFontMetrics() const;
    int logFontWidth(int length) const;
    int logFontWidth(const QString& text) const;

    int getSourceStringMaxWidth(int size);

signals:
    void signalInvalidated();

public:
    const int MAX_SOURCE_WIDTH = 50;

    int m_maxSourcePixelWidth = 300;
    int m_currentSourcePixelWidth = 0;

    int m_maxRowsInLogModel = 100000;
    int m_numberOfBytesToTail = 100000;
    QString m_logThreshold = Statics::LogSeverityFilterOff;
    int m_logFontHeight;
    int m_logViewSpacing = 2;
    QString m_style;
    int m_sourceDataWidth = 0;

    bool m_showSource = false;

private:
    QFont m_logFont;
    QFont m_appFont;

};
