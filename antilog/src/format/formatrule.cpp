#include "formatrule.h"
#include "statics.h"

#include <QObject>
#include <QJsonObject>
#include <QMetaProperty>

FormatRule::FormatRule(const QJsonObject& json) : QObject()
{
    if (!json.empty())
    {
        m_columnType = GlobalColumn::staticGetTypeFromString(json["scope"].toString());
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

QString FormatRule::getSearchTerm() const
{
    return m_searchTerm;
}

GlobalColumn::ColumnType FormatRule::getColumnType() const
{
    return m_columnType;
}

void FormatRule::save(QJsonObject& json) const
{
    json["scope"] = GlobalColumn::staticGetTypeAsString(m_columnType);
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
    return m_color = static_cast<Color>(Statics::metaIndex(staticMetaObject, "ColorProperty", color));
}

FormatRule::Color FormatRule::setBgColorFromString(const QString& color)
{
    return m_bgcolor = static_cast<Color>(Statics::metaIndex(staticMetaObject, "BgColorProperty", color));
}

FormatRule::MatchingRule FormatRule::setMatchingRuleFromString(const QString& operation)
{
    return m_matchingRule =
            static_cast<MatchingRule>(Statics::metaIndex(staticMetaObject, "MatchingRuleProperty", operation));
}

FormatRule::FormatArea FormatRule::setFormatAreaFromString(const QString& range)
{
    return m_formatArea =
            static_cast<FormatArea>(Statics::metaIndex(staticMetaObject, "FormatAreaProperty", range));
}

QStringList FormatRule::getColorList()
{
    QStringList ret;
    auto meta =
            staticMetaObject.property(staticMetaObject.indexOfProperty("ColorProperty")).enumerator();

    for(int i = 0; i < meta.keyCount(); i++)
    {
        ret << meta.key(i);
    }
    return ret;
}

QStringList FormatRule::getMatchingRules()
{
    QStringList ret;
    auto meta =
            staticMetaObject.property(staticMetaObject.indexOfProperty("MatchingRuleProperty")).enumerator();

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

RuleHit* FormatRule::findLogHit(const QString &cell)
{
    RuleHit* ruleHit = nullptr;
    const QString& cellTerm = m_case ? cell : cell.toLower();
    const QString& target = m_case ? m_searchTerm : m_searchTerm.toLower();

    if (m_matchingRule == MatchingRule::Contains)
    {
        int index = cell.indexOf(target);
        if (index >= 0)
        {
            switch(m_formatArea)
            {
            case FormatArea::Word:
                ruleHit = new RuleHit(index, index + target.size(), this);
                break;
            case FormatArea::Cell:
                ruleHit = new RuleHit(0, cell.size(), this);
                break;
            case FormatArea::Line:
                ruleHit = new RuleHit(-1, -1, this);
                break;
            }
        }
    }
    else // Operation::Equals
    {
        if (cellTerm == target)
        {
            ruleHit = new RuleHit(-1, -1, this);
        }
    }
    return ruleHit;
}

QString FormatRule::getTag() const
{
    return colorAsString() + bgColorAsString();
}

QString FormatRule::getCSS(const QString &tag) const
{
    QString css;

    if (m_formatArea == FormatArea::Line)
    {
        css += "tr";
    }
    else
    {
        css += "span." + tag;
    }

    css += "{color: " + colorAsString() +";background-color:" + bgColorAsString() + ";";

    if (m_formatArea == FormatArea::Line)
    {
        css += "display: block;";
    }
    return css + "}";
}

QString FormatRule::getPreHtml(const QString &tag) const
{
    return "<span class=\"" + tag + "\">";
}

QString FormatRule::getPostHtml() const
{
    return "</span>";
}


