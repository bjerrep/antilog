#pragma once

#include "globalcolumnconfig.h"
#include "statics.h"
#include "options.h"

#include <QObject>
#include <QJsonObject>
#include <QVector>

extern int s_columnUid;

class SchemeColumn : public QObject
{
    Q_OBJECT

public:
    SchemeColumn(GlobalColumn::ColumnType cellType, bool enabled = false);
    SchemeColumn(const QJsonObject& json);
    SchemeColumn(const SchemeColumn&);

    void save(QJsonObject& json) const;
    void load(const QJsonObject& json);

    GlobalColumn::ColumnType getCellType() const;
    QString getTypeAsString() const;
    bool isEnabled() const;
    int getUid() const;

private:
    GlobalColumn::ColumnType m_columnType = GlobalColumn::ColumnType::UNKNOWN;
    bool m_enabled;
    int m_uid = s_columnUid++;

    friend class SchemeColumnModel;
};

