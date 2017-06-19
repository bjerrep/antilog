#include "sourcevisitors.h"
#include "filesourcedialog.h"
#include "passprocessordialog.h"
#include "regexprocessordialog.h"
#include "udpsourcedialog.h"
#include "logentryformatter.h"
#include "formatscheme.h"

#include <QComboBox>
#include <QIcon>

void GetName::visit(FileSource* fileSource)
{
    m_name = fileSource->getName();
}

void GetName::visit(UDPSource* udpSource)
{
    m_name = udpSource->getName();
}

void GetName::visit(PassProcessor* passProcessor)
{
    m_name = passProcessor->getName();
}

void GetName::visit(RegexProcessor* regexProcessor)
{
    m_name = regexProcessor->getName();
}

// ------ GetJson -------

void GetJson::visit(FileSource *fileSource)
{
    fileSource->save(m_json);
}

void GetJson::visit(UDPSource *udpSource)
{
    udpSource->save(m_json);
}

void GetJson::visit(PassProcessor* passProcessor)
{
    passProcessor->save(m_json);
}

void GetJson::visit(RegexProcessor* regexProcessor)
{
    regexProcessor->save(m_json);
}

// ------ GetLabel -------

GetLabel::GetLabel()
{
}

void GetLabel::visit(FileSource*)
{
    Statics::widgetIcon(Statics::FileResource, this);
}

void GetLabel::visit(UDPSource*)
{
    Statics::widgetIcon(Statics::UDPSourceResource, this);
}

void GetLabel::visit(PassProcessor*)
{
    Statics::widgetIcon(Statics::PassProcessorResource, this);
}

void GetLabel::visit(RegexProcessor*)
{
    Statics::widgetIcon(Statics::RegexProcessorResource, this);
}

// ------ GetInputDialogWidget -------

GetInputDialogWidget::GetInputDialogWidget()
{
    ui = new Ui::inputWidget();
    resize(400, 200);

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    setSizePolicy(sizePolicy);

    ui->setupUi(this);
}

GetInputDialogWidget::~GetInputDialogWidget()
{
    delete ui;
}

void GetInputDialogWidget::visit(FileSource *fileSource)
{
    ui->labelIcon->setPixmap(Statics::pixmapIcon(Statics::FileResource));
    ui->labelName->setText(fileSource->getName());
    ui->labelDetail->setText(fileSource->getDescription());
}

void GetInputDialogWidget::visit(UDPSource *udpSource)
{
    ui->labelIcon->setPixmap(Statics::pixmapIcon(Statics::UDPSourceResource));
    ui->labelName->setText(udpSource->getName());
    ui->labelDetail->setText(udpSource->getDescription());
}

void GetInputDialogWidget::visit(PassProcessor* passProcessor)
{
    ui->labelIcon->setPixmap(Statics::pixmapIcon(Statics::PassProcessorResource));
    ui->labelName->setText(passProcessor->getName());
    ui->labelDetail->setText(passProcessor->getDescription());
}

void GetInputDialogWidget::visit(RegexProcessor* regexProcessor)
{
    ui->labelIcon->setPixmap(Statics::pixmapIcon(Statics::RegexProcessorResource));
    ui->labelName->setText(regexProcessor->getName());
    ui->labelDetail->setText(regexProcessor->getDescription());
}

// ------ GetDialog -------

GetDialog::GetDialog()
{
}

void GetDialog::visit(FileSource *fileSource)
{
    FileSourceDialog temp(*fileSource, this);
}

void GetDialog::visit(UDPSource *udpSource)
{
    UdpSourceDialog temp(udpSource, this);
}

void GetDialog::visit(PassProcessor* passProcessor)
{
    PassProcessorDialog temp(passProcessor, this);
}

void GetDialog::visit(RegexProcessor* regexProcessor)
{
    RegexProcessorDialog temp(regexProcessor, this);
}
