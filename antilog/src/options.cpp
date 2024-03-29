#include "options.h"
#include "column/globalcolumnconfig.h"

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

    opt["appFontFamily"] = m_appFont.family();
    opt["appFontPoint"] = m_appFont.pointSize();
    opt["appFontBold"] = m_appFont.bold();
    opt["appFontItallic"] = m_appFont.italic();

    opt["logFontFamily"] = m_logFont.family();
    opt["logFontPoint"] = m_logFont.pointSize();
    opt["logFontBold"] = m_logFont.bold();
    opt["logFontItallic"] = m_logFont.italic();

    opt["showsource"] = m_showSource;
    json["options"] = opt;
}

void Options::setAppFont(QFont font)
{
    m_appFont = font;
}

QFont& Options::getAppFont()
{
    return m_appFont;
}

void Options::setLogFont(QFont font)
{
    m_logFont = font;
    QFontMetrics fontMetrics(m_logFont);
    m_logFontHeight = fontMetrics.height();
}

QFont& Options::getLogFont()
{
    return m_logFont;
}

QFontMetrics Options::logFontMetrics() const
{
    return QFontMetrics(m_logFont);
}

int Options::logFontWidth(int length) const
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    return logFontMetrics().horizontalAdvance(QString(length, '0'));
#else
    return logFontMetrics().width(QString(length, '0'));
#endif
}

int Options::logFontWidth(const QString& text) const
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    return logFontMetrics().horizontalAdvance(text);
#else
    return logFontMetrics().width(text);
#endif
}

int Options::getSourceStringMaxWidth(int size)
{
    if (size > MAX_SOURCE_WIDTH)
    {
        m_sourceDataWidth = MAX_SOURCE_WIDTH;
        emit signalInvalidated();
    }
    else if (size > m_sourceDataWidth)
    {
        m_sourceDataWidth = size;
        emit signalInvalidated();
    }
    return m_sourceDataWidth;
}

void Options::load(const QJsonObject& json)
{
    if (json.empty() || json["options"].toObject().isEmpty())
    {
        m_appFont = QFont("Noto", 9);
        m_logFont = QFont("Noto", 9);
        return;
    }

    auto opt = json["options"].toObject();
    m_numberOfBytesToTail = opt["numberOfBytesToTail"].toInt();
    m_maxRowsInLogModel = opt["maxRowsInLogModel"].toInt();
    m_logThreshold = opt["logThreshold"].toString();

    m_appFont = QFont(opt["appFontFamily"].toString(), opt["appFontPoint"].toInt());
    m_appFont.setBold(opt["appFontBold"].toBool());
    m_appFont.setItalic(opt["appFontItallic"].toBool());

    auto logFont = QFont(opt["logFontFamily"].toString(), opt["logFontPoint"].toInt());
    logFont.setBold(opt["logFontBold"].toBool());
    logFont.setItalic(opt["logFontItallic"].toBool());
    setLogFont(logFont);

    m_showSource = opt["showsource"].toBool();
}
