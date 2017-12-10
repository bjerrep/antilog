#include "options.h"
#include "globalcolumnconfig.h"

#include <QFontMetrics>

Options::Options(const QJsonObject& json)
{
    load(json);
}

Options::~Options()
{
}

void Options::save(QJsonObject& json) const
{
    QJsonObject opt;
    opt["numberOfBytesToTail"] = m_numberOfBytesToTail;
    opt["maxRowsInLogModel"] = m_maxRowsInLogModel;
    opt["logThreshold"] = m_logThreshold;
    opt["logFontFamily"] = m_logFont.family();
    opt["logFontPoint"] = m_logFont.pointSize();
    opt["appFontFamily"] = m_appFont.family();
    opt["appFontPoint"] = m_appFont.pointSize();
    opt["showsource"] = m_showSource;
    json["options"] = opt;
}

void Options::setLogFont(const QString& family, int point)
{
    m_logFont = QFont(family, point);
    QFontMetrics fontMetrics(m_logFont);
    m_logFontHeight = fontMetrics.height();
}

QFontMetrics Options::logFontMetrics() const
{
    return QFontMetrics(m_logFont);
}

int Options::logFontWidth(int length) const
{
    return logFontMetrics().width(QString(length, '0'));
}

int Options::getSourceStringMaxWidth(int size)
{
    if (size > m_sourceDataWidth && m_sourceDataWidth < MAX_WIDTH)
    {
        m_sourceDataWidth = qMin(size, MAX_WIDTH);
        emit signalInvalidated();
    }
    return m_sourceDataWidth;
}

void Options::load(const QJsonObject& json)
{
    if (json.empty() || json["options"].toObject().isEmpty())
    {
        m_appFont = QFont("Noto", 9);
        setLogFont("Source Code Pro", 9);

        return;
    }

    auto opt = json["options"].toObject();
    m_numberOfBytesToTail = opt["numberOfBytesToTail"].toInt();
    m_maxRowsInLogModel = opt["maxRowsInLogModel"].toInt();
    m_logThreshold = opt["logThreshold"].toString();

    setLogFont(opt["logFontFamily"].toString(), opt["logFontPoint"].toInt());
    m_appFont = QFont(opt["appFontFamily"].toString(), opt["appFontPoint"].toInt());
    m_showSource = opt["showsource"].toBool();
}
