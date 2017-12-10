#include "formatschememodel.h"
#include "formatscheme.h"
#include "globalcolumnconfig.h"
#include "statics.h"
#include "formatrule.h"
#include "options.h"

FormatSchemes::FormatSchemes(const QJsonObject& json, GlobalColumnConfig* columnnLibrary)
    : m_columnLibrary(columnnLibrary)
{
    m_formatSchemeList.append(new FormatScheme(Statics::NoneScheme, this));
    load(json);
}

FormatSchemes::~FormatSchemes()
{
    qDeleteAll(m_formatSchemeList);
}

void FormatSchemes::load(const QJsonObject& json)
{
    auto inputs = json["formatmodel"].toArray();
    foreach (auto input, inputs)
    {
        auto obj = input.toObject();
        auto formatScheme = new FormatScheme(obj, this);
        m_formatSchemeList.append(formatScheme);
    }
}

void FormatSchemes::save(QJsonObject& json) const
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

FormatScheme* FormatSchemes::getFormatScheme(const QString& name)
{
    foreach (auto scheme, m_formatSchemeList)
    {
        if (scheme->getName() == name)
            return scheme;
    }
    return m_formatSchemeList.first();
}

const FormatSchemeList& FormatSchemes::getFormatSchemes() const
{
    return m_formatSchemeList;
}

QStringList FormatSchemes::getSchemeNames()
{
    QStringList names;
    foreach (auto formatScheme, m_formatSchemeList)
    {
        names << formatScheme->getName();
    }
    return names;
}

int FormatSchemes::getSchemeNameIndex(const QString& name)
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

void FormatSchemes::addScheme(const QString& name)
{
    m_formatSchemeList.append(new FormatScheme(name, this));
}

void FormatSchemes::deleteScheme(const QString& name)
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

GlobalColumnConfig *FormatSchemes::getGlobalColumns()
{
    return m_columnLibrary;
}
