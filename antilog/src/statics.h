#pragma once

#include <QString>
#include <QPixmap>
#include <QDebug>

class QWidget;
class LogFormatModel;
class FormatSchemes;
class LogSeverityCategories;
class Options;
class AntiLog;
class GlobalColumnConfig;

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

    extern const QString& DefaultDateName;
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

    GlobalColumnConfig* instColumnLibrary();
    Options* instOptions();
    AntiLog* instAntiLog();

    extern GlobalColumnConfig* m_globalColumnConfig;
    extern Options* s_options;
    extern AntiLog* s_antiLog;

    extern LogSeverityCategories* s_logSeverities;

    int widgetIcon(const QString& resourceImage, QWidget* widget, int height = Statics::InputIconHeight);
    QPixmap pixmapIcon(const QString& resourceImage, int height = Statics::InputIconHeight);
    QWidget* widgetIcon(const QString& resourceImage, int height = Statics::InputIconHeight);
    int metaIndex(const QMetaObject& metaObject, const char* var, const QString& key);
}

#define warn(message) qWarning() << "[" << __BASE_FILE__ << "@" << __func__ << "] " << message;
