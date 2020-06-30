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

    m_logFont = QFont(opt["logFontFamily"].toString(), opt["logFontPoint"].toInt());
    m_logFont.setBold(opt["logFontBold"].toBool());
    m_logFont.setItalic(opt["logFontItallic"].toBool());

    m_showSource = opt["showsource"].toBool();
}
