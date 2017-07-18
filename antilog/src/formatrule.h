#pragma once

#include "logentryformatter.h"
#include "statics.h"

#include <QObject>

class LogEntryFormatter;

/// The FormatRule objects are configured by FormatWidgets in the FormatDialog.
/// A FormatRule is owned by a FormatScheme which has a list of FormatRules.
/// It is the FormatRule that constructs LogEntryFormatter objects which can be part
/// of the LogEntry objects.
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

    QString searchTerm() const;
    LogEntryFormatterPtr match(const QStringList& cells, const QString& formatschemeName, const QStringList& scopes) const;
    static QStringList getColorList();
    static QStringList getMatchingRules();
    static QStringList getFormatAreas(bool multiRow);

private:
    QString m_scope = Statics::FormatColumnAny;
    MatchingRule m_matchingRule = MatchingRule::Contains;
    QString m_searchTerm = "undefined";
    Color m_color = Color::Black;
    Color m_bgcolor = Color::White;
    bool m_bold = true;
    bool m_case = false;
    FormatArea m_formatArea = FormatArea::Word;

    Q_PROPERTY(MatchingRule matchingRule MEMBER m_matchingRule)
    Q_PROPERTY(Color color MEMBER m_color)
    Q_PROPERTY(Color bgcolor MEMBER m_bgcolor)
    Q_PROPERTY(FormatArea formatArea MEMBER m_formatArea)

    friend class FormatWidget;
};

