#pragma once

#include <QMap>
#include <QVector>
#include <QFileSystemWatcher>

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
    FileReader(const QString& filename, int lastNumlines = 0);
    FileReader(const QString& rootDir, bool recursive, const QString& mask);
    ~FileReader();

    void setup();

    static QStringList tailFileByLines(const QString& filename, int lines, qint64& pos);
    static int glob(const QString& directory,
                    QStringList& files,
                    QStringList& directories,
                    const QString& mask = "*",
                    bool recurse = true);

private:
    QString sourceIdentification(const QString& filename) const;

public slots:
    void startReader();

private slots:
    void slotFileChanged(const QString& path);
    void slotDirectoryChanged(const QString& directory);
    void slotCheckForMissingFile(const QString& directory);

signals:
    void signalNewData(QString data, QString sourceIdentifier);

private:
    QString m_filename;
    QString m_rootDir;
    bool m_recursive = false;
    QString m_mask;
    int m_lastNumLines = 0;
    QMap<QString, qint64> m_positions;
    QFileSystemWatcher m_watcher;
    QStringList m_missingFiles;
    QFileSystemWatcher m_missingFilesWatcher;
    QThread* m_thread = nullptr;
    Mode m_mode;
};
