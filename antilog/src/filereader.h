#pragma once

#include <QObject>

class QFileSystemWatcher;
class QThread;

class FileReader : public QObject
{
    Q_OBJECT

public:
    FileReader(const QString& filename, int lastNumlines = 0);
    ~FileReader();

    bool isRunning();
    static QStringList tailFileByLines(const QString& filename, int lines, qint64& pos);

public slots:
    void systemReady();
    void slotFileChanged(const QString &path);
    void start();

signals:
    void signalNewData(QString data);

private:
    QString m_filename;
    int m_lastNumLines;
    qint64 m_pos = -1;
    QFileSystemWatcher* m_watcher = nullptr;
    QThread* m_thread = nullptr;
};
