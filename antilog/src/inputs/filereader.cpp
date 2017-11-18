#include "filereader.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QFileSystemWatcher>
#include <QThread>

/// Start a file reader on the given file. If the file is not found
/// the reader will start once the file shows up after startReader() is called.
/// The reader does not deal with files dissapearing and then returning again.
/// Once constructed connect to signalNewData and call startReader()
///
FileReader::FileReader(const QString& filename, int lastNumlines)
    : m_filename(filename),
      m_lastNumLines(lastNumlines),
      m_mode(FILE)
{
    setup();
}

/// Start a file reader on all files below rootdir with the given mask, optionally recursive.
/// All directories seen will be added to a file watcher which will add new files in the
/// directories if they match the mask.
/// Once constructed connect to signalNewData and call startReader()
///
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
    m_thread->requestInterruption();
}

void FileReader::setup()
{
    connect(&m_watcher, &QFileSystemWatcher::fileChanged,
            this, &FileReader::slotFileChanged);
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged,
            this, &FileReader::slotDirectoryChanged);

    connect(&m_missingFilesWatcher, &QFileSystemWatcher::directoryChanged,
            this, &FileReader::slotCheckForMissingFile);

    // threaded since startReader() will block if the last part of a file
    // should be loaded right away
    m_thread = new QThread;
    moveToThread(m_thread);
    m_thread->start();
    connect(m_thread, &QThread::finished,
            m_thread, &QThread::deleteLater);
}

QString FileReader::sourceIdentification(const QString& filename) const
{
    return QFileInfo(filename).fileName();
}

int FileReader::glob(const QString& directory,
                     QStringList& files,
                     QStringList& directories,
                     const QString& mask,
                     bool recurse)
{
    QDir dirObj(directory);

    if (!dirObj.exists())
    {
        return -1;
    }
    QStringList filterList = {mask};
    QFileInfoList fileInfoList = dirObj.entryInfoList(filterList, QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    foreach (auto fileInfo, fileInfoList)
    {
        if (fileInfo.isDir() && recurse)
        {
            glob(fileInfo.absoluteFilePath(), files, directories, mask, recurse);
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

    if (m_mode == FILE)
    {
        files << m_filename;
    }
    else
    {
        QStringList directories;
        glob(m_rootDir, files, directories, m_mask, m_recursive);
        m_watcher.addPath(m_rootDir);
        foreach (auto directory, directories)
        {
            m_watcher.addPath(directory);
        }
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

            m_watcher.addPath(file);
        }
        else
        {
            if (m_mode == FILE)
            {
                m_missingFiles.append(file);
                auto directory = QFileInfo(file).absolutePath();
                m_missingFilesWatcher.addPath(directory);
            }
        }
    }
}

QStringList FileReader::tailFileByLines(const QString& filename, int lines, qint64& pos)
{
    // SO/25823778. Buggy, needs unittesting. And to be rewritten for speed.
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        file.seek(file.size() - 2);

        bool terminate = false;
        while (!terminate)
        {
            if (file.read(1) == "\n")
            {
                --lines;
            }
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

void FileReader::slotDirectoryChanged(const QString& directory)
{
    QStringList files, directories;

    glob(directory, files, directories, m_mask, false);

    auto existingFiles = m_watcher.files();
    foreach (auto file, files)
    {
        if (!existingFiles.contains(file))
        {
            auto fqn = QDir(directory).filePath(file);
            m_watcher.addPath(fqn);
            return;
        }
    }
}

void FileReader::slotCheckForMissingFile(const QString& directory)
{
    foreach (auto missingFile, m_missingFiles)
    {
        if (QFileInfo(missingFile).absolutePath() == directory)
        {
            m_watcher.addPath(missingFile);
            m_missingFilesWatcher.removePath(directory);
            m_missingFiles.takeAt(m_missingFiles.indexOf(missingFile));
            return;
        }
    }
}
