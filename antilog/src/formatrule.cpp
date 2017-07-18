#include "formatrule.h"
#include "logentryformatter.h"
#include "statics.h"
#include "tableformat.h"

#include <QObject>
#include <QJsonObject>
#include <QMetaProperty>

FormatRule::FormatRule(const QJsonObject& json) : QObject()
{
    if (!json.empty())
    {
        m_scope = json["scope"].toString();
        setMatchingRuleFromString(json["operation"].toString());
        m_searchTerm = json["searchterm"].toString();
        setColorFromString(json["color"].toString());
        setBgColorFromString(json["bgcolor"].toString());
        m_bold = json["bold"].toBool();
        m_case = json["case"].toBool();
        setFormatAreaFromString(json["formatarea"].toString());
    }
}

FormatRule::FormatRule(MatchingRule key, const QString& value)
    : m_matchingRule(key),
      m_searchTerm(value)
{
}

QString FormatRule::searchTerm() const
{
    return m_searchTerm;
}

void FormatRule::save(QJsonObject& json) const
{
    json["scope"] = m_scope;
    json["operation"] = matchingRuleAsString();
    json["searchterm"] = m_searchTerm;
    json["color"] = colorAsString();
    json["bgcolor"] = bgColorAsString();
    json["bold"] = m_bold;
    json["case"] = m_case;
    json["formatarea"] = formatAreaAsString();
}

QString FormatRule::colorAsString() const
{
    return QVariant::fromValue(m_color).toString();
}

QString FormatRule::bgColorAsString() const
{
    return QVariant::fromValue(m_bgcolor).toString();
}

QString FormatRule::matchingRuleAsString() const
{
    return QVariant::fromValue(m_matchingRule).toString();
}

QString FormatRule::formatAreaAsString() const
{
    return QVariant::fromValue(m_formatArea).toString();
}

FormatRule::Color FormatRule::setColorFromString(const QString& color)
{
    return m_color = static_cast<Color>(Statics::metaIndex(staticMetaObject, "color", color));
}

FormatRule::Color FormatRule::setBgColorFromString(const QString& color)
{
    return m_bgcolor = static_cast<Color>(Statics::metaIndex(staticMetaObject, "bgcolor", color));
}

FormatRule::MatchingRule FormatRule::setMatchingRuleFromString(const QString& operation)
{
    return m_matchingRule = static_cast<MatchingRule>(Statics::metaIndex(staticMetaObject, "matchingRule", operation));
}

FormatRule::FormatArea FormatRule::setFormatAreaFromString(const QString& range)
{
    return m_formatArea = static_cast<FormatArea>(Statics::metaIndex(staticMetaObject, "formatArea", range));
}

LogEntryFormatterPtr FormatRule::match(const QStringList& logEntries,
                                       const QString& formatschemeName,
                                       const QStringList& logCellTitles) const
{
    QList<int> rowHits;

    int start = 0;
    int end = logCellTitles.size() - 1;

    // find the column to search in if a specific one is selected
    if (m_scope != Statics::FormatColumnAny)
    {
        start = end = logCellTitles.indexOf(m_scope);
    }
    end = qMin(end, logEntries.size() - 1);

    const QString& searchTerm = m_case ? m_searchTerm : m_searchTerm.toLower();

    for(int i = start; i <= end; i++)
    {
        const QString& logEntry = m_case ? logEntries.at(i) : logEntries.at(i).toLower();

        if (m_matchingRule == MatchingRule::Contains)
        {
            if (logEntry.indexOf(searchTerm) >= 0)
            {
                rowHits.append(i);
            }
        }
        else // Operation::Equals
        {
            if (logEntry == searchTerm)
            {
                rowHits.append(i);
            }
        }
    }

    if (rowHits.size() == 0)
        return LogEntryFormatterPtr();

    LogEntryFormatterPtr logEntryFormatter(new LogEntryFormatter(formatschemeName));

    end = qMin(logCellTitles.size(), logEntries.size());

    for(int i = 0; i < end; i++)
    {
        const QString subMessage = logEntries.at(i);
        LogCellFormatter logCellFormatter(subMessage);

        if (m_formatArea == FormatArea::Word)
        {
            logCellFormatter.wordMode(searchTerm, m_case);
        }

        if (m_formatArea == FormatArea::Line || rowHits.contains(i))
        {
            if (m_bold)
            {
                logCellFormatter.setBold();
            }
            if (m_color != Color::Black)
            {
                const QString color = QVariant::fromValue(m_color).toString();
                logCellFormatter.setColor(color);
            }
            if (m_bgcolor != Color::White)
            {
                const QString color = QVariant::fromValue(m_bgcolor).toString();
                logCellFormatter.setBackgroundColor(color);
            }
        }
        logEntryFormatter->addLogCellFormatter(logCellFormatter);
    }

    return logEntryFormatter;
}

QStringList FormatRule::getColorList()
{
    QStringList ret;
    auto meta = staticMetaObject.property(staticMetaObject.indexOfProperty("color")).enumerator();

    for(int i = 0; i < meta.keyCount(); i++)
    {
        ret << meta.key(i);
    }
    return ret;
}

QStringList FormatRule::getMatchingRules()
{
    QStringList ret;
    auto meta = staticMetaObject.property(staticMetaObject.indexOfProperty("matchingRule")).enumerator();

    for(int i = 0; i < meta.keyCount(); i++)
    {
        ret << meta.key(i);
    }
    return ret;
}

QStringList FormatRule::getFormatAreas(bool multiRow)
{
    QStringList ret;
    ret << QVariant::fromValue(FormatArea::Word).toString();
    if (multiRow)
    {
        ret << QVariant::fromValue(FormatArea::Cell).toString();
    }
    ret << QVariant::fromValue(FormatArea::Line).toString();
    return ret;
}
