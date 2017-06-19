#pragma once

#include "inputbases.h"

#include <QString>
#include <QJsonObject>

// ------ PassProcessor -------

class PassProcessor : public ProcessorBase
{
    Q_OBJECT

public:
    PassProcessor(const QJsonObject& json = QJsonObject());

    void save(QJsonObject& json) const override;
    void accept(InputVisitorBase* v);
    void setSchemeFromName(const QString& scheme) override;

public slots:
    void slotNewData(InputItemBase* source, QString data);
};

// ------ RegexProcessor -------

class RegexProcessor : public ProcessorBase
{
    Q_OBJECT

public:
    RegexProcessor(const QJsonObject& json = QJsonObject());

    void save(QJsonObject& json) const override;
    void accept(InputVisitorBase* v);
    QStringList applyRegex(const QString& data);
    QString getRegex() const;
    void setRegex(const QString& getRegex);
    int getNofEnabledColumns() const;
    void setSchemeFromName(const QString& schemeName);

private:
    void updateDescription();

public slots:
    void slotNewData(InputItemBase* source, QString data);

private:
    const char* RegexKey = "regex";
    const char* OnlyMatchesKey = "onlypassmatches";

    QString m_regex;
    int m_nofEnabledRows = 0;
    bool m_onlyPassRegexMatches = true;

    friend class RegexProcessorDialog;
};

// ------ Statics -------

void getAllProcessors(InputItemList& list);
