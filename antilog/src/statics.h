#pragma once

#include <QString>
#include <QPixmap>
#include <QDebug>

class QWidget;
class LogFormatModel;
class FormatSchemeModel;
class LogSeverityCategories;
struct Options;
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

    extern const QString& DefaultDateName; // remove - only used for default init
    extern const QString& DefaultTimeName;
    extern const QString& DefaultSeverityName;
    extern const QString& DefaultModuleIdName;
    extern const QString& DefaultTextName;
    extern const QString& DefaultAnyName;

    extern const QString& NoneScheme;

    extern const QString& LogSeverityFilterOff;
    extern const QString& LogSeverityFilterOverruled;

    extern const int InputIconHeight;
    extern const int InputIconVerticalMargin;

    const int LogViewMargin = 8;

    extern bool SystemReady;

    extern const QString& AntiLogMessage;

    extern FormatSchemeModel* s_formatSchemeModel;
    extern LogSeverityCategories* s_logSeverities;
    extern Options* s_options;
    extern AntiLog* s_antiLog;

    Options* getOptions();

    int widgetIcon(const QString& resourceImage, QWidget* widget, int height = Statics::InputIconHeight);
    QPixmap pixmapIcon(const QString& resourceImage, int height = Statics::InputIconHeight);
    QWidget* widgetIcon(const QString& resourceImage, int height = Statics::InputIconHeight);
    int metaIndex(const QMetaObject& metaObject, const char* var, const QString& key);
}

#define warn(message) qWarning() << "[" << __BASE_FILE__ << "@" << __func__ << "] " << message;
