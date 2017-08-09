#include "formatschememodel.h"
#include "tableformat.h"
#include "statics.h"
#include "formatrule.h"
#include "options.h"

FormatSchemeModel::FormatSchemeModel(const QJsonObject& json)
{
    load(json);
    m_entries.append(new FormatScheme("None"));
}

void FormatSchemeModel::load(const QJsonObject& json)
{
    auto inputs = json["formatmodel"].toArray();
    foreach (auto input, inputs)
    {
        auto obj = input.toObject();
        auto formatScheme = new FormatScheme(obj);
        m_entries.append(formatScheme);
    }
}

void FormatSchemeModel::save(QJsonObject& json) const
{
    QJsonArray ret;
    foreach (auto setup, m_entries)
    {
        if (setup->name() != Statics::NoneScheme)
        {
            QJsonObject obj;
            setup->save(obj);
            ret.append(obj);
        }
    }
    if (!ret.empty())
    {
        json["formatmodel"] = ret;
    }
}

FormatScheme* FormatSchemeModel::getFormatScheme(const QString& name)
{
    foreach (auto scheme, m_entries)
    {
        if (scheme->name() == name)
            return scheme;
    }
    return m_entries.first();
}

const FormatSchemeList& FormatSchemeModel::getFormatSchemes() const
{
    return m_entries;
}

void FormatSchemeModel::constructTestSchemes()
{
    auto formatScheme = new FormatScheme("Default");
    formatScheme->addFormatRule(new FormatRule(FormatRule::MatchingRule::Contains, "verbose"));
    formatScheme->addFormatRule(new FormatRule(FormatRule::MatchingRule::Equals, "release"));
    m_entries.append(formatScheme);
}

QStringList FormatSchemeModel::getSchemeNames()
{
    QStringList names;
    foreach (auto formatScheme, m_entries)
    {
        names << formatScheme->name();
    }
    return names;
}

int FormatSchemeModel::getSchemeNameIndex(const QString& name)
{
    for(int i = 0; i < m_entries.size(); i++)
    {
        if (name == m_entries.at(i)->name())
        {
            return i;
        }
    }
    return -1;
}

void FormatSchemeModel::addScheme(const QString& name)
{
    m_entries.append(new FormatScheme(name));
}

void FormatSchemeModel::deleteScheme(const QString& name)
{
    if (name == Statics::NoneScheme)
    {
        throw "Can't delete the none scheme";
    }
    int i = getSchemeNameIndex(name);

    if (i >= 0)
    {
        delete m_entries.takeAt(i);
    }
}

void FormatSchemeModel::slotNewLogFont()
{
    foreach(auto scheme, m_entries)
    {
        scheme->getTableFormat().recalculate(Statics::s_options->logFontMetrics());
    }
}
