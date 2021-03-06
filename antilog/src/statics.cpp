#include "statics.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QMetaProperty>

namespace Statics
{
    const QString& UDPSourceResource = ":/artwork/artwork/udp.svg";
    const QString& UDPMulticastSourceResource = ":/artwork/artwork/multicast.svg";
    const QString& FileResource = ":/artwork/artwork/file.svg";
    const QString& DirResource = ":/artwork/artwork/dir.svg";

    const QString& PassProcessorResource = ":/artwork/artwork/pass.svg";
    const QString& RegexProcessorResource = ":/artwork/artwork/regex.svg";

    const QString& ClassName = "type";
    const QString& Name = "name";
    const QString& Description = "description";

    const QString& FilePath = "filepath";
    const QString& Source = "source";
    const QString& Processor = "processor";
    const QString& Enabled = "enabled";
    const QString& Inputs = "inputs";
    const QString& TailOnly = "tailonly";

    const QString& DefaultDateName = "Date";
    const QString& DefaultTimeName = "Time";
    const QString& DefaultSeverityName = "Severity";
    const QString& DefaultModuleIdName = "ModuleId";
    const QString& DefaultTextName = "Message";
    const QString& DefaultAnyName = "Any";

    const QString& NoneScheme = "None";

    const QString& LogSeverityFilterOff = "All";
    const QString& LogSeverityFilterOverruled = "-";

    const int InputIconHeight = 50;
    const int InputIconVerticalMargin = 8;

    bool SystemReady = false;

    Options *instOptions()
    {
        return s_options;
    }

    AntiLog *instAntiLog()
    {
        return s_antiLog;
    }

    GlobalColumnConfig* instColumnLibrary()
    {
        return m_globalColumnConfig;
    }

    const QString& AntiLogMessage = "AntiLog Message: ";

    LogSeverityCategories* s_logSeverities = nullptr;
    Options* s_options = nullptr;
    AntiLog* s_antiLog = nullptr;
    GlobalColumnConfig* m_globalColumnConfig = nullptr;


    QPixmap pixmapIcon(const QString& resourceImage, int height)
    {
        QPixmap icon(resourceImage);
        // hack to counteract something below that don't work as planned
        height -= 2;
        double fac = double(height) / icon.height();
        int width = icon.width() * fac;
        icon = icon.scaled(width, icon.height() * fac, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        return icon;
    }

    int widgetIcon(const QString& resourceImage, QWidget* widget, int height)
    {
        QHBoxLayout* layout = new QHBoxLayout();
        widget->setLayout(layout);
        QLabel* label = new QLabel();
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);
        QPixmap pixmap = pixmapIcon(resourceImage, height);
        label->setPixmap(pixmap);
        return pixmap.width();
    }

    QWidget* widgetIcon(const QString& resourceImage, int height)
    {
        auto* widget = new QWidget();
        widgetIcon(resourceImage, widget, height);
        return widget;
    }

    int metaIndex(const QMetaObject& metaObject, const char* var, const QString& key)
    {
        auto meta = metaObject.property(metaObject.indexOfProperty(var)).enumerator();
        return meta.keyToValue(key.toLatin1());
    }
}
