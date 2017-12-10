#include "formatscheme.h"
#include "formatschememodel.h"
#include "formatrule.h"


FormatScheme::FormatScheme(const QJsonObject& json, FormatSchemes *parentModel)
    : m_parent(parentModel)
{
    if (json.empty())
    {
        return;
    }
    m_name = json["name"].toString();
    auto inputs = json["setup"].toArray();
    foreach (auto input, inputs)
    {
        auto scheme = input.toObject();
        auto formatEntry = new FormatRule(scheme);
        m_formatRuleList.append(formatEntry);
    }
    m_schemeColumnModel = new SchemeColumnModel(json);
}

FormatScheme::FormatScheme(const QString& name, FormatSchemes *parentModel)
    : m_name(name),
      m_parent(parentModel)
{
    m_schemeColumnModel = new SchemeColumnModel();
}

FormatScheme::~FormatScheme()
{
    qDeleteAll(m_formatRuleList);
    delete m_schemeColumnModel;
}

void FormatScheme::save(QJsonObject& json) const
{
    QJsonArray result;
    foreach (auto formatEntry, m_formatRuleList)
    {
        QJsonObject cssScheme;
        formatEntry->save(cssScheme);
        result.append(cssScheme);
    }
    json["setup"] = result;
    json["name"] = m_name;
    m_schemeColumnModel->save(json);
}

QString FormatScheme::getName() const
{
    return m_name;
}

FormatRuleList& FormatScheme::getEntries()
{
    return m_formatRuleList;
}

bool FormatScheme::hasEntry(const QString& id)
{
    foreach (auto formatRule, m_formatRuleList)
    {
        if (id == formatRule->getColumnType())
        {
            return true;
        }
    }
    return false;
}

void FormatScheme::addFormatRule(FormatRule* formatRule)
{
    m_formatRuleList.append(formatRule);
}

void FormatScheme::deleteFormatRule(FormatRule* formatRule)
{
    int i = m_formatRuleList.indexOf(formatRule);

    if (i != -1)
    {
        delete m_formatRuleList.takeAt(i);
    }
}

QString FormatScheme::tableHtml(
        const QStringList& logCells,
        const QString& sourceData,
        const FormatRuleList& formatRuleList,
        const GlobalColumnTypeList& columnTypeList,
        const GlobalColumnConfig* tableFormat)
{
    int cellIndex = 0;
    QString css;
    QString html;

    for (auto cell: logCells)
    {
        RuleHits* ruleHits = 0;

        for (auto rule: formatRuleList)
        {
            if (rule->getColumnType() == columnTypeList[cellIndex] || rule->getColumnType() == GlobalColumn::ANY)
            {
                RuleHit* ruleHit = rule->findLogHit(cell);
                if (ruleHit)
                {
                    if (!ruleHits)
                    {
                        ruleHits = new RuleHits();
                    }
                    ruleHits->m_ruleHitList.append(ruleHit);
                }
            }
        }

        if (ruleHits)
        {
            const int nofHits = ruleHits->m_ruleHitList.size();
            for (int i = 0; i < nofHits; i++)
            {
                const RuleHit* hit = ruleHits->m_ruleHitList[i];
                const QString& tag = hit->m_rule->getTag();
                css += hit->m_rule->getCSS(tag);
                const QString& pre = hit->m_rule->getPreHtml(tag);
                const QString& post = hit->m_rule->getPostHtml();
                cell.insert(hit->m_end, post);
                cell.insert(hit->m_start, pre);
            }
        }

        if (logCells.size() == columnTypeList.size())
        {
            html += tableFormat->getTableDataHtml(cell, columnTypeList[cellIndex]);
        }
        else
        {
            html += tableFormat->getTableDataHtml(cell, GlobalColumn::UNKNOWN);
        }
        cellIndex++;
    }

    if (!css.isEmpty())
    {
        css = "<style>" + css + "</style>";
    }
    html = "<html>" + css + "<table><tr>" + sourceData + html + "</tr></table></html>";
    return html;
}

QString FormatScheme::tableText(const QStringList &logCells,
                                const QString &sourceData,
                                const GlobalColumnTypeList &columnTypeList)
{
    QString text;
    int index = 0;

    for (auto cell: logCells)
    {
        GlobalColumn::ColumnType tpe = columnTypeList[index];
        int width = m_parent->getGlobalColumns()->getTableCellFormatMap()[tpe]->getLength();
        if (width <= 0)
            text += cell + " ";
        else
            text += cell.leftJustified(width, ' ', true) + " ";
        index++;
    }
    return sourceData + text;
}

QString FormatScheme::getTableRowHtml(const QStringList& logEntries, QString sourceName)
{
    if (!sourceName.isEmpty())
    {
        int sourceWidth = Statics::instOptions()->getSourceStringMaxWidth(sourceName.size());
        sourceName = sourceName.left(sourceWidth);
        int widthInPixels = Statics::instOptions()->logFontWidth(sourceWidth);
        sourceName = QString("<td width=%1><b><small>%2</small></b></td>").arg(widthInPixels).arg(sourceName);
    }

    QString html = tableHtml(logEntries,
                             sourceName,
                             m_formatRuleList,
                             m_schemeColumnModel->getEnabledColumnTypes(),
                             m_parent->getGlobalColumns());
    return html;
}

QString FormatScheme::getTableRowText(const QStringList &logEntries, QString sourceName)
{
    if (!sourceName.isEmpty())
    {
        int sourceWidth = Statics::instOptions()->getSourceStringMaxWidth(sourceName.size());
        sourceName = sourceName.left(sourceWidth) + " ";
    }

    QString text = tableText(logEntries,
                             sourceName,
                             m_schemeColumnModel->getEnabledColumnTypes());
    return text;
}

SchemeColumnModel& FormatScheme::getColumnSetup()
{
    return *m_schemeColumnModel;
}

