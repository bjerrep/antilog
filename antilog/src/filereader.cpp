#include "filereader.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QFileSystemWatcher>
#include <QThread>

FileReader::FileReader(const QString& filename, int lastNumlines)
    : m_filename(filename),
      m_lastNumLines(lastNumlines),
      m_mode(FILE)
{
    setup();
}

FileReader::FileReader(const QString& rootDir, bool recursive, const QString& mask)
    : m_rootDir(rootDir),
      m_recursive(recursive),
      m_mask(mask),
      m_mode(DIR)
{
    setup();
}

FileReader::~FileReader()
{
    if (m_watcher)
    {
        m_watcher->deleteLater();
    }
    m_thread->requestInterruption();
    m_thread->deleteLater();
}

void FileReader::setup()
{
    m_watcher = new QFileSystemWatcher();
    connect(m_watcher, &QFileSystemWatcher::fileChanged,
            this, &FileReader::slotFileChanged);

    m_thread = new QThread;
    moveToThread(m_thread);
    m_thread->start();
}

void FileReader::systemReady()
{
    startReader();
}

QString FileReader::sourceIdentification(const QString& filename) const
{
    return QFileInfo(filename).fileName();
}

int FileReader::glob(const QString& dir, QStringList& files, QStringList& directories, const QString& filter, bool recurse)
{
    QDir dirObj(dir);
    QStringList filterList = {filter};
    QFileInfoList fileInfoList = dirObj.entryInfoList(filterList, QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    foreach (auto fileInfo, fileInfoList)
    {
        if (fileInfo.isDir() && recurse)
        {
            glob(fileInfo.absoluteFilePath(), files, directories, filter, recurse);
        }

        if (fileInfo.isFile())
        {
            files.append(fileInfo.absoluteFilePath());
        }
        else if (fileInfo.isDir())
        {
            directories.append(fileInfo.absoluteFilePath());
        }
    }

    int noAccess = 0;
    foreach (auto file, files)
    {
        noAccess += !QFileInfo(file).isReadable();
    }
    return noAccess;
}

void FileReader::startReader()
{
    QStringList files;
    QStringList directories;

    if (m_mode == FILE)
    {
        files << m_filename;
    }
    else
    {
        glob(m_rootDir, files, directories, m_mask, m_recursive);
    }

    foreach (auto file, files)
    {
        if (QFile(file).exists())
        {
            m_positions[file] = QFile(file).size();

            if (m_lastNumLines)
            {
                QStringList lines = tailFileByLines(file, m_lastNumLines, m_positions[file]);
                foreach (auto line, lines)
                {
                    emit signalNewData(line, sourceIdentification(file));
                }
            }

            m_watcher->addPath(file);
        }
    }
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

void FileReader::slotFileChanged(const QString& filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        qint64 pos = m_positions[filename];

        file.seek(pos);
        while (true)
        {
            QString data = file.readLine();
            pos = file.pos();
            if (!data.isEmpty())
            {
                emit signalNewData(data, sourceIdentification(filename));
            }
            else
            {
                break;
            }
        }
        file.close();
        m_positions[filename] = pos;
    }
}





