#pragma once

#include <QString>
#include <QPixmap>
#include <QDebug>

class QWidget;
class LogFormatModel;
class FormatSchemeModel;
class LogLevelCategories;
class Options;
class AntiLog;

namespace Statics
{
    // Sources
    extern const QString& UDPSourceResource;
    extern const QString& FileResource;
    extern const QString& DirResource;

    // Processors
    extern const QString& PassProcessorResource;
    extern const QString& RegexProcessorResource;

    extern const QString& ClassName;
    extern const QString& Name;
    extern const QString& Description;

    extern const QString& FilePath;
    extern const QString& Source;
    extern const QString& Processor;
    extern const QString& Enabled;
    extern const QString& Inputs;
    extern const QString& TailOnly;

    extern const QString& Date;
    extern const QString& Time;
    extern const QString& Level;
    extern const QString& Id;
    extern const QString& Message;

    extern const QString& NoneScheme;

    extern const QString& FormatColumnAny;

    extern const QString& logLevelFilterOff;
    extern const QString& logLevelFilterOverruled;

    extern const int iconHeight;
    extern const int verticalMargin;

    const int LogViewMargin = 8;

    extern bool SystemReady;

    extern const QString& AntiLogMessage;

    extern FormatSchemeModel* formatSchemeModel;
    extern LogLevelCategories* logLevels;
    extern Options* options;
    extern AntiLog* antiLog;

    int widgetIcon(const QString& resourceImage, QWidget* widget, int height = Statics::iconHeight);
    QPixmap pixmapIcon(const QString& resourceImage, int height = Statics::iconHeight);
    QWidget* widgetIcon(const QString& resourceImage, int height = Statics::iconHeight);
    int metaIndex(const QMetaObject& metaObject, const char* var, const QString& key);
}

#define warn(message) qWarning() << "[" << __BASE_FILE__ << "@" << __func__ << "] " << message;
