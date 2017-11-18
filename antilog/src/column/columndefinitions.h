#pragma once

#include <QObject>
#include <QJsonObject>
#include <QVector>


extern int s_columnUid;

class Column : public QObject
{
    Q_OBJECT

public:

    enum ColumnType
    {
        UNKNOWN,
        DATE,
        TIME,
        MODULEID,
        SEVERITY,
        TEXT,
        ANY
    };
    Q_ENUM(ColumnType)

    Column();
    Column(ColumnType cellType, const QString& name, bool enabled = false);
    Column(const QString& columnTypeName, const QString& name, bool enabled, int uid);
    Column(const QJsonObject& json);
    Column(const Column&);

    void save(QJsonObject& json) const;
    void load(const QJsonObject& json);

    ColumnType getCellType() const;
    QString getCellTypeAsString() const;
    static QString getCellTypeAsString(ColumnType columnType);
    static ColumnType getCellTypeFromString(const QString& columnTypeName);

    QString getName() const;
    bool isEnabled() const;
    int getUid() const;
    void setDeleted();

private:
    ColumnType m_columnType = ColumnType::UNKNOWN;
    QString m_name;
    bool m_enabled;
    int m_uid = s_columnUid++;
    bool m_deleted = false;

    Q_PROPERTY(ColumnType ColumnTypeProperty MEMBER m_columnType)

    friend class ColumnDefinitions;
};

using ColumnList = QVector<Column*>;
using ColumnTypeList = QVector<Column::ColumnType>;


class ColumnDefinitions : public QObject
{
    Q_OBJECT

public:
    ColumnDefinitions(const QJsonObject& json);
    ~ColumnDefinitions();

    void save(QJsonObject& json);

    ColumnList getColumnTypeList() const;
    QStringList getColumnTypeStringlist() const;

    void reset();
    Column* findUid(int uid);
    void refreshColumns(ColumnList& columnList);

signals:
    void signalInvalidated();

private:
    void clear();

private:
    ColumnList m_columnList;
};
