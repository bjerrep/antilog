#include "filereader.h"

#include <QFile>
#include <QApplication>
#include <QFileSystemWatcher>
#include <QThread>

FileReader::FileReader(const QString& filename, int lastNumlines)
    : m_filename(filename),
      m_lastNumLines(lastNumlines)
{
    m_thread = new QThread;
    moveToThread(m_thread);
        m_thread->start();
}

void FileReader::systemReady()
{
    start();
}

void FileReader::start()
{
    if (QFile(m_filename).exists())
    {
        m_pos = QFile(m_filename).size();

        if (m_lastNumLines)
        {
            QStringList lines = tailFileByLines(m_filename, m_lastNumLines, m_pos);
            foreach (auto line, lines)
            {
                emit signalNewData(line);
            }
        }

        m_watcher = new QFileSystemWatcher();
        m_watcher->addPath(m_filename);
        connect(m_watcher, &QFileSystemWatcher::fileChanged,
                this, &FileReader::slotFileChanged);
    }
}

FileReader::~FileReader()
{
    if (m_watcher)
    {
        m_watcher->deleteLater();
    }
    m_thread->quit();
    m_thread->deleteLater();
}

bool FileReader::isRunning()
{
    return m_watcher;
}

QStringList FileReader::tailFileByLines(const QString& filename, int lines, qint64& pos)
{
    // SO/25823778. Buggy, needs unittesting. And to be rewritten for speed.
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        file.seek(file.size() - 2);

        bool terminate = false;
        while (!terminate)
        {
            if (file.read(1) == "\n")
                --lines;
            terminate = !lines || (file.pos() <= 1);
            if (!terminate)
            {
                file.seek(file.pos() - 2);
            }
            else
            {
                file.seek(file.pos() - 1);
            }
        }
        QStringList lines;

        while (true)
        {
            QString data = file.readLine();
            if (!data.isEmpty())
            {
                lines.append(data);
            }
            else
            {
                pos = file.pos();
                break;
            }
        }
        file.close();
        return lines;
    }
    return QStringList();
}

void FileReader::slotFileChanged(const QString &path)
{
    QFile file(m_filename);
    if (file.open(QIODevice::ReadOnly))
    {
        file.seek(m_pos);
        while (true)
        {
            QString data = file.readLine();
            m_pos = file.pos();
            if (!data.isEmpty())
            {
                emit signalNewData(data);
            }
            else
            {
                break;
            }
        }
        file.close();
    }
}





