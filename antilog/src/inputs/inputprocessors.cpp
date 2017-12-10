#include "inputprocessors.h"
#include "sourcevisitors.h"
#include "formatscheme.h"
#include "logentry.h"

PassProcessor::PassProcessor(const QJsonObject& json)
    : ProcessorBase(__func__, json)
{
    if (!json.empty())
    {
        QString schemeName = json[SchemeNameKey].toString();
        setSchemeFromName(schemeName);
    }
    else
    {
        setName(makeNameUnique("Pass"));
        setSchemeFromName(Statics::NoneScheme);
    }
}

void PassProcessor::save(QJsonObject& json) const
{
    ProcessorBase::save(json);
    json[SchemeNameKey] = getScheme()->getName();
}

void PassProcessor::accept(InputVisitorBase* v)
{
    v->visit(this);
}

void PassProcessor::setSchemeFromName(const QString& schemeName)
{
    ProcessorBase::setSchemeFromName(schemeName);
    setDescription("Format scheme '" + schemeName + "'");
}

void PassProcessor::slotNewData(SourceBase* source, QString data, QString sourceIdentifier)
{
    setInputItem(source);
    auto logEntry = LogEntryPtr(new LogEntry(data, sourceIdentifier, getScheme()));
    signalNewProcessorData(this, logEntry);
}

// ------ RegexProcessor -------

RegexProcessor::RegexProcessor(const QJsonObject& json)
    : ProcessorBase(__func__, json)
{
    if (!json.empty())
    {
        QString schemeName = json[SchemeNameKey].toString();
        setSchemeFromName(schemeName);
        m_regex = json[RegexKey].toString();
        m_onlyPassRegexMatches = json[OnlyMatchesKey].toBool();
    }
    else
    {
        setName(makeNameUnique("Regex"));
        setSchemeFromName(Statics::NoneScheme);
        // matches spdlog
        m_regex = "\\[(\\S+) (\\S+)\\] \\[(\\S+)\\] \\[(\\S+)\\] (.*)";
    }

    updateDescription();
}

void RegexProcessor::save(QJsonObject& json) const
{
    ProcessorBase::save(json);
    json[SchemeNameKey] = getScheme()->getName();
    json[RegexKey] = m_regex;
    json[OnlyMatchesKey] = m_onlyPassRegexMatches;
}

QString RegexProcessor::getRegex() const
{
    return m_regex;
}

void RegexProcessor::setRegex(const QString& regex)
{
    m_regex = regex;
    updateDescription();
    m_nofEnabledRows = getScheme()->getColumnSetup().getNofEnabledColumns();
}

int RegexProcessor::getNofEnabledColumns() const
{
    return m_nofEnabledRows;
}

void RegexProcessor::accept(InputVisitorBase* v)
{
    v->visit(this);
}

QStringList RegexProcessor::applyRegex(const QString& data)
{
    QRegExp re(m_regex);
    re.indexIn(data);
    auto captures = re.capturedTexts();
    if (re.matchedLength() == -1)
    {
        return QStringList();
    }
    captures.removeFirst();
    return captures;
}

void RegexProcessor::updateDescription()
{
    setDescription("Format scheme '" + getScheme()->getName() + "', " + m_regex);
}

void RegexProcessor::setSchemeFromName(const QString& schemeName)
{
    ProcessorBase::setSchemeFromName(schemeName);
    updateDescription();
}

void RegexProcessor::slotNewData(SourceBase* source, QString data, QString sourceIdentifier)
{
    setInputItem(source);
    auto rows = applyRegex(data);

    if (getScheme()->getColumnSetup().getNofEnabledColumns() == rows.count())
    {
        auto logEntryPtr = LogEntryPtr(new LogEntry(rows, sourceIdentifier, getScheme()));
        signalNewProcessorData(this, logEntryPtr);
    }
    else if (!m_onlyPassRegexMatches)
    {
        auto logEntryPtr = LogEntryPtr(new LogEntry(data, sourceIdentifier, getScheme()));
        signalNewProcessorData(this, logEntryPtr);
    }
}

// ------ getAllProcessorTypes -------

void getAllProcessorTypes(InputItemList& list)
{
    list.append(new PassProcessor());
    list.append(new RegexProcessor());
}
