#pragma once

#include <QObject>
#include <QMap>

class QFileSystemWatcher;
class QThread;

class FileReader : public QObject
{
    Q_OBJECT

    enum Mode
    {
        FILE,
        DIR
    };

public:
    FileReader(const QString& filename, const QString& sourceName, int lastNumlines = 0);
    FileReader(const QString& rootDir, bool recursive, const QString& mask, const QString& sourceName);
    ~FileReader();
    void setup();

    static QStringList tailFileByLines(const QString& filename, int lines, qint64& pos);
    static int glob(const QString& dir, QStringList& files, QStringList& directories,
                    const QString& filter = "*", bool recurse = true);

private:
    bool isRunning();
    QString sourceIdentification(const QString& filename) const;

public slots:
    void systemReady();
    void slotFileChanged(const QString &path);
    void startReader();

signals:
    void signalNewData(QString data, QString sourceIdentifier);

private:
    QString m_filename;
    QString m_rootDir;
    bool m_recursive = false;
    QString m_mask;
    QString m_sourceName;
    int m_lastNumLines = 0;
    QMap<QString, qint64> m_positions;
    QFileSystemWatcher* m_watcher = nullptr;
    QThread* m_thread = nullptr;
    Mode m_mode;
};
