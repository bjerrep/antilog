#pragma once

#include <QObject>
#include <QJsonObject>

/// A specific columntype with a ColumnType and a human readable name.
/// See GlobalColumnConfig
///
class GlobalColumn : public QObject
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

    GlobalColumn(const ColumnType& columnType, const QString& name, const QString& html1, const QString& html2, int width);
    GlobalColumn(const QJsonObject& json);

    void save(QJsonObject& json) const;

    QString getHtmlTableDataStart() const;
    QString getHtmlTableDataEnd() const;

    void setName(const QString& name);
    QString getName() const;
    ColumnType getType() const;
    int getLength() const;

    void setTypeFromString(const QString& typeString);
    QString getTypeAsString() const;

    bool isEnabled() const;

    static QString staticGetTypeAsString(GlobalColumn::ColumnType columnType);
    static ColumnType staticGetTypeFromString(const QString& columnTypeName);

private:
    ColumnType m_columnType;
    QString m_name;
    QString m_htmlTableDataStart;
    QString m_htmlTableDataEnd;
    int m_length;
    bool m_enabled = true;

    Q_PROPERTY(ColumnType ColumnTypeProperty MEMBER m_columnType)

    friend class ColumnBasicTableFormat;
};

using GlobalColumnList = QVector<GlobalColumn*>;

