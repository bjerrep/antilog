#pragma once

#include "statics.h"
#include "column/schemecolumn.h"

#include <QObject>

class FormatRule;

class RuleHit
{
public:
    RuleHit(int start, int end, FormatRule* rule)
        : m_start(start),
          m_end(end),
          m_rule(rule)
    {}

    int m_start;
    int m_end;
    FormatRule* m_rule;
};

using RuleHitList = QVector<RuleHit*>;

class RuleHits
{
public:
    RuleHitList m_ruleHitList;
};


/// A FormatRule knows how to generate a RuleHits object for a text and how to html format it afterwards.
/// FormatRules are configured by FormatWidgets in the FormatDialog.
/// A FormatRule is owned by a FormatScheme which has a list of FormatRules.
///
class FormatRule : public QObject
{
    Q_OBJECT

public:
    enum class MatchingRule
    {
        Equals,
        Contains
    };
    Q_ENUM(MatchingRule)

    enum class Color
    {
        Black,
        White,
        Red,
        Blue,
        Green,
        Yellow
    };
    Q_ENUM(Color)

    enum class FormatArea
    {
        Word,
        Cell,
        Line
    };
    Q_ENUM(FormatArea)

public:
    FormatRule(const QJsonObject& json);
    FormatRule(MatchingRule key = MatchingRule::Contains, const QString& value = "undefined");
    void save(QJsonObject& json) const;

    QString colorAsString() const;
    QString bgColorAsString() const;
    QString matchingRuleAsString() const;
    QString formatAreaAsString() const;

    Color setColorFromString(const QString& color);
    Color setBgColorFromString(const QString& color);
    MatchingRule setMatchingRuleFromString(const QString& operation);
    FormatArea setFormatAreaFromString(const QString& range);

    QString getSearchTerm() const;
    GlobalColumn::ColumnType getColumnType() const;
    static QStringList getColorList();
    static QStringList getMatchingRules();
    static QStringList getFormatAreas(bool multiRow);

    RuleHit* findLogHit(const QString& cell);
    QString getTag() const;
    QString getCSS(const QString& tag) const;
    QString getPreHtml(const QString& tag) const;
    QString getPostHtml() const;

private:
    GlobalColumn::ColumnType m_columnType = GlobalColumn::ANY;
    MatchingRule m_matchingRule = MatchingRule::Contains;
    QString m_searchTerm = "undefined";
    Color m_color = Color::Black;
    Color m_bgcolor = Color::White;
    bool m_bold = true;
    bool m_case = false;
    FormatArea m_formatArea = FormatArea::Word;

    Q_PROPERTY(MatchingRule MatchingRuleProperty MEMBER m_matchingRule)
    Q_PROPERTY(Color ColorProperty MEMBER m_color)
    Q_PROPERTY(Color BgColorProperty MEMBER m_bgcolor)
    Q_PROPERTY(FormatArea FormatAreaProperty MEMBER m_formatArea)

    friend class FormatWidget;
};

using FormatRuleList = QVector<FormatRule*>;
