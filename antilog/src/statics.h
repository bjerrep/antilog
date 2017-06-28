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
    extern const char* UDPSourceResource;
    extern const char* FileResource;
    extern const char* DirResource;

    // Processors
    extern const char* PassProcessorResource;
    extern const char* RegexProcessorResource;

    extern const char* ClassName;
    extern const char* Name;
    extern const char* Description;

    extern const char* FilePath;
    extern const char* Source;
    extern const char* Processor;
    extern const char* Enabled;
    extern const char* Inputs;
    extern const char* TailOnly;

    extern const char* Date;
    extern const char* Time;
    extern const char* Level;
    extern const char* Id;
    extern const char* Message;

    extern const char* NoneScheme;

    extern const char* FormatColumnAny;

    extern const char* logLevelFilterOff;

    extern const int iconHeight;
    extern const int verticalMargin;

    const int LogViewMargin = 8;

    extern bool SystemReady;

    extern FormatSchemeModel* formatSchemeModel;
    extern LogLevelCategories* logLevels;
    extern Options* options;
    extern AntiLog* antiLog;

    int widgetIcon(QString resourceImage, QWidget* widget, int height = Statics::iconHeight);
    QPixmap pixmapIcon(QString resourceImage, int height = Statics::iconHeight);
    QWidget* widgetIcon(QString resourceImage, int height = Statics::iconHeight);
}

#define warn(message) qWarning() << "[" << __BASE_FILE__ << "::" << __func__ << "] " << message;
