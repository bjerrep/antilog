#include "statics.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

namespace Statics
{
    const char* UDPSourceResource = ":/artwork/artwork/udp.svg";
    const char* FileResource = ":/artwork/artwork/file.svg";

    const char* PassProcessorResource = ":/artwork/artwork/pass.svg";
    const char* RegexProcessorResource = ":/artwork/artwork/regex.svg";

    const char* ClassName = "type";
    const char* Name = "name";
    const char* Description = "description";

    const char* FilePath = "filepath";
    const char* Source = "source";
    const char* Processor = "processor";
    const char* Enabled = "enabled";
    const char* Inputs = "inputs";
    const char* TailOnly = "tailonly";

    const char* Date = "Date";
    const char* Time = "Time";
    const char* Level = "Level";
    const char* Id = "Id";
    const char* Message = "Message";

    const char* NoneScheme = "None";

    const char* FormatColumnAny = "Any";

    const char* logLevelFilterOff = "All";

    const int iconHeight = 50;
    const int verticalMargin = 8;

    bool SystemReady = false;

    FormatSchemeModel* formatSchemeModel = nullptr;
    LogLevelCategories* logLevels = nullptr;
    Options* options = nullptr;
    AntiLog* antiLog = nullptr;


    QPixmap pixmapIcon(QString resourceImage, int height)
    {
        QPixmap icon(resourceImage);
        // hack to counteract something below that don't work as planned
        height -= 2;
        double fac = double(height) / icon.height();
        int width = icon.width() * fac;
        icon = icon.scaled(width, icon.height() * fac, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        return icon;
    }

    int widgetIcon(QString resourceImage, QWidget* widget, int height)
    {
        QHBoxLayout* layout = new QHBoxLayout();
        widget->setLayout(layout);
        QLabel* label = new QLabel();
        label->setAlignment(Qt::AlignHCenter);
        layout->addWidget(label);
        QPixmap pixmap = pixmapIcon(resourceImage, height);
        label->setPixmap(pixmap);
        return pixmap.width();
    }

    QWidget* widgetIcon(QString resourceImage, int height)
    {
        auto* widget = new QWidget();
        widgetIcon(resourceImage, widget, height);
        return widget;
    }
}
