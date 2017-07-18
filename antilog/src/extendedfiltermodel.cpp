#include "extendedfiltermodel.h"
#include "statics.h"
#include "loglevels.h"
#include "extendedwidget.h"

#include <QJsonObject>
#include <QJsonArray>

ExtendedFilterModel::ExtendedFilterModel(const QJsonObject& json)
{
    QJsonObject model = json["extendedfilters"].toObject();
    m_filterModelIsActive = model["active"].toBool();
    m_persistSourcesAndModules = model["m_persist"].toBool();

    QJsonArray sources = model["sources"].toArray();
    foreach (auto source, sources)
    {
        QJsonArray modules = source.toObject()["modules"].toArray();
        ExtendedFilterItem* sourceItem = new ExtendedFilterItem(source.toObject(), nullptr);
        foreach (auto module, modules)
        {
            ExtendedFilterItem* moduleItem = new ExtendedFilterItem(module.toObject(), sourceItem);
            sourceItem->addModule(moduleItem);
        }
        m_sources.append(sourceItem);
    }
}

void ExtendedFilterModel::save(QJsonObject& json) const
{
    QJsonObject model;
    model["active"] = m_filterModelIsActive;
    model["m_persist"] = m_persistSourcesAndModules;

    if (m_persistSourcesAndModules)
    {
        QJsonArray sourceArray;
        foreach (auto source, m_sources)
        {
            QJsonArray array;
            foreach (auto module, source->getModules())
            {
                QJsonObject obj;
                module->save(obj);
                array.append(obj);
            }
            QJsonObject sourceObj;
            source->save(sourceObj);
            sourceObj["modules"] = array;
            sourceArray.append(sourceObj);
        }
        model["sources"] = sourceArray;
    }
    json["extendedfilters"] = model;
}

void ExtendedFilterModel::setActive(bool active)
{
    m_filterModelIsActive = active;
}

void ExtendedFilterModel::registerLogEntry(LogEntryPtr logEntry)
{
    if (!m_filterModelIsActive)
    {
        // should entries be registered even if the filter model is not active ? Not for now...
        return;
    }

    auto moduleName = logEntry->getModuleName();
    if (moduleName.isNull())
    {
        return;
    }

    auto sourceName = logEntry->getSourceName();

    foreach (auto sourceFilterItem, m_sources)
    {
        if (sourceFilterItem->getName() == sourceName)
        {
            // the source already exist
            foreach (auto moduleChild, sourceFilterItem->getModules())
            {
                if (moduleChild->getName() == moduleName)
                {
                    return;
                }
            }
            // new module
            QString severity = sourceFilterItem->getSeverity() != Statics::logLevelFilterOverruled ?
                        sourceFilterItem->getSeverity() :
                        getDefaultSeverity();
            auto moduleFilterItem = new ExtendedFilterItem(moduleName, m_newSourcesEnabled, severity, sourceFilterItem);
            sourceFilterItem->addModule(moduleFilterItem);
            emit signalNewFilterItems(sourceFilterItem, moduleFilterItem);
            return;
        }
    }

    // new source
    auto sourceFilterItem = new ExtendedFilterItem(sourceName, m_newSourcesEnabled, getDefaultSeverity(), nullptr);
    auto moduleFilterItem = new ExtendedFilterItem(moduleName, m_newSourcesEnabled, getDefaultSeverity(), sourceFilterItem);
    sourceFilterItem->addModule(moduleFilterItem);
    m_sources.append(sourceFilterItem);
    emit signalNewFilterItems(sourceFilterItem, moduleFilterItem);
}

bool ExtendedFilterModel::isMatched(LogEntryPtr logEntry) const
{
    if (!m_filterModelIsActive)
    {
        return true;
    }

    foreach (auto source, m_sources)
    {
        if (source->getName() == logEntry->getSourceName())
        {
            auto moduleName = logEntry->getModuleName();
            foreach (auto module, source->getModules())
            {
                if (module->getName() == moduleName)
                {
                    bool severityMatched = logEntry->isInScope(module->getSeverity(), QString());
                    return module->getEnableState() && severityMatched;
                }
            }
        }
    }
    return true;
}

void ExtendedFilterModel::enableAll(Qt::CheckState enable)
{
    foreach (auto source, m_sources)
    {
        source->setEnableState(enable);
        foreach (auto source, source->getModules())
        {
            source->setEnableState(enable);
        }
    }
}

ExtendedFilterItem* ExtendedFilterModel::sourceAt(int index)
{
    return m_sources[index];
}

ExtendedFilterItem* ExtendedFilterModel::findModule(ExtendedFilterItem* toFind) const
{
    foreach (auto source, m_sources)
    {
        if (source == toFind)
        {
            return source;
        }
        foreach (auto module, source->getModules())
        {
            if (module == toFind)
            {
                return module;
            }
        }
    }
    return nullptr;
}

QString ExtendedFilterModel::getDefaultSeverity() const
{
    return m_defaultSeverity;
}

