#include "formatschememodel.h"
#include "formatscheme.h"
#include "statics.h"
#include "formatrule.h"
#include "options.h"

FormatSchemeModel::FormatSchemeModel(const QJsonObject& json)
{
    m_formatSchemeList.append(new FormatScheme(Statics::NoneScheme));
    load(json);
}

FormatSchemeModel::~FormatSchemeModel()
{
    qDeleteAll(m_formatSchemeList);
}

void FormatSchemeModel::load(const QJsonObject& json)
{
    auto inputs = json["formatmodel"].toArray();
    foreach (auto input, inputs)
    {
        auto obj = input.toObject();
        auto formatScheme = new FormatScheme(obj);
        m_formatSchemeList.append(formatScheme);
    }
}

void FormatSchemeModel::save(QJsonObject& json) const
{
    QJsonArray ret;
    foreach (auto setup, m_formatSchemeList)
    {
        if (setup->getName() != Statics::NoneScheme)
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
    foreach (auto scheme, m_formatSchemeList)
    {
        if (scheme->getName() == name)
            return scheme;
    }
    return m_formatSchemeList.first();
}

const FormatSchemeList& FormatSchemeModel::getFormatSchemes() const
{
    return m_formatSchemeList;
}

QStringList FormatSchemeModel::getSchemeNames()
{
    QStringList names;
    foreach (auto formatScheme, m_formatSchemeList)
    {
        names << formatScheme->getName();
    }
    return names;
}

int FormatSchemeModel::getSchemeNameIndex(const QString& name)
{
    for(int i = 0; i < m_formatSchemeList.size(); i++)
    {
        if (name == m_formatSchemeList.at(i)->getName())
        {
            return i;
        }
    }
    return -1;
}

void FormatSchemeModel::addScheme(const QString& name)
{
    m_formatSchemeList.append(new FormatScheme(name));
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
        delete m_formatSchemeList.takeAt(i);
    }
}

void FormatSchemeModel::slotNewLogFont()
{
    foreach(auto scheme, m_formatSchemeList)
    {
        scheme->getTableFormat().recalculate(Statics::getOptions()->logFontMetrics());
    }
}
