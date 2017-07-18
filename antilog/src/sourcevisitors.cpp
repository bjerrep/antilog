#include "sourcevisitors.h"
#include "filesourcedialog.h"
#include "dirsourcedialog.h"
#include "passprocessordialog.h"
#include "regexprocessordialog.h"
#include "udpsourcedialog.h"
#include "logentryformatter.h"
#include "formatscheme.h"

#include <QComboBox>
#include <QIcon>

void GetName::visit(FileSource* visitor)
{
    m_name = visitor->getName();
}

void GetName::visit(DirSource* visitor)
{
    m_name = visitor->getName();
}

void GetName::visit(UDPSource* visitor)
{
    m_name = visitor->getName();
}

void GetName::visit(PassProcessor* visitor)
{
    m_name = visitor->getName();
}

void GetName::visit(RegexProcessor* visitor)
{
    m_name = visitor->getName();
}

// ------ GetJson -------

void GetJson::visit(FileSource* visitor)
{
    visitor->save(m_json);
}

void GetJson::visit(DirSource* visitor)
{
    visitor->save(m_json);
}

void GetJson::visit(UDPSource* visitor)
{
    visitor->save(m_json);
}

void GetJson::visit(PassProcessor* visitor)
{
    visitor->save(m_json);
}

void GetJson::visit(RegexProcessor* visitor)
{
    visitor->save(m_json);
}

// ------ GetLabel -------

GetLabel::GetLabel()
{
}

void GetLabel::visit(FileSource*)
{
    Statics::widgetIcon(Statics::FileResource, this);
}

void GetLabel::visit(DirSource*)
{
    Statics::widgetIcon(Statics::DirResource, this);
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

void GetInputDialogWidget::visit(FileSource* fileSource)
{
    ui->labelIcon->setPixmap(Statics::pixmapIcon(Statics::FileResource));
    ui->labelName->setText(fileSource->getName());
    ui->labelDetail->setText(fileSource->getDescription());
}

void GetInputDialogWidget::visit(DirSource* dirSource)
{
    ui->labelIcon->setPixmap(Statics::pixmapIcon(Statics::DirResource));
    ui->labelName->setText(dirSource->getName());
    ui->labelDetail->setText(dirSource->getDescription());
}

void GetInputDialogWidget::visit(UDPSource* udpSource)
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

void GetDialog::visit(FileSource* visitor)
{
    FileSourceDialog temp(*visitor, this);
}

void GetDialog::visit(DirSource* visitor)
{
    DirSourceDialog temp(*visitor, this);
}

void GetDialog::visit(UDPSource* visitor)
{
    UdpSourceDialog temp(visitor, this);
}

void GetDialog::visit(PassProcessor* visitor)
{
    PassProcessorDialog temp(visitor, this);
}

void GetDialog::visit(RegexProcessor* visitor)
{
    RegexProcessorDialog temp(visitor, this);
}
