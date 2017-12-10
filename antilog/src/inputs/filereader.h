#pragma once

#include <QMap>
#include <QFileSystemWatcher>
#include <QThread>


class FileReaderThread : public QObject
{
    Q_OBJECT

public slots:
    void tailFileByBytes(const QString& filename, int bytes, qint64& pos);

signals:
    void signalNewData(QString data, QString sourceIdentifier);
};


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
    QString getSourcename(const QString& filename) const;

public slots:
    void startReader();

private slots:
    void slotFileChanged(const QString& path);
    void slotDirectoryChanged(const QString& directory);
    void slotCheckForMissingFile(const QString& directory);
    void slotNewFileReaderData(const QString& data, const QString& sourceIdentifier);

signals:
    void signalNewData(QString data, QString sourceIdentifier);

private:
    QString m_filename;
    int m_nofBytesToTail = 0;
    QString m_rootDir;
    bool m_recursive = false;
    QString m_mask;

    QMap<QString, qint64> m_positions;
    QFileSystemWatcher m_watcher;
    QStringList m_missingFiles;
    QFileSystemWatcher m_missingFilesWatcher;
    Mode m_mode;
    QThread m_fileReaderThread;
};
