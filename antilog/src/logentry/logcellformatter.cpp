#include "logcellformatter.h"

#include <QJsonObject>
#include <QVector>

LogCellFormatter::LogCellFormatter(const QString& name)
    : m_name(name)
{
}

QString LogCellFormatter::getCellHtml(QString message) const
{
    if (m_wordIndexes.size() == 0)
    {
        return m_preHtml + message + m_postHtml;
    }

    int added = m_preHtml.size() + m_postHtml.size();
    int offset = 0;
    foreach (int i, m_wordIndexes)
    {
        i += offset;
        message.insert(i, m_preHtml);
        message.insert(i + m_preHtml.size() + m_length, m_postHtml);
        offset += added;
    }

    return message;
}

QString LogCellFormatter::getStylesheet() const
{
    return m_styleSheet;
}

void LogCellFormatter::wordMode(const QString& word, bool caseSensitive)
{
    int i = 0;
    while(i >= 0)
    {
        if (caseSensitive)
            i = m_name.indexOf(word, i);
        else
            i = m_name.toLower().indexOf(word, i);

        if (i >= 0)
        {
            m_wordIndexes.append(i++);
        }
    }
    m_length = word.size();
}

void LogCellFormatter::setBold()
{
    m_preHtml += "<b>";
    m_postHtml = "</b>" + m_postHtml;
}

void LogCellFormatter::setColor(const QString& color)
{
    m_preHtml += "<font color='" + color + "'>";
    m_postHtml = "</font>" + m_postHtml;
}

void LogCellFormatter::setBackgroundColor(const QString& color)
{
    m_preHtml += "<span class=\"" + color + "\">";
    m_postHtml += "</span>";
    m_styleSheet = "<style> span." + color + "{background-color: " + color + ";}</style>";
}
