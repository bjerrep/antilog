#pragma once

#include "inputsources.h"
#include "inputprocessors.h"
#include "inputvisitorbase.h"

#include <QJsonObject>
#include <QWidget>

class AntiLog;
namespace Ui {
    class inputWidget;
}

class GetName : public InputVisitorBase
{
public:
    void visit(FileSource* fileSource);
    void visit(DirSource* dirSource);
    void visit(UDPSource* udpSource);
    void visit(UDPMulticastSource* udpSource);
    void visit(PassProcessor* passProcessor);
    void visit(RegexProcessor* regexProcessor);

private:
    QString m_name;
};


class GetJson : public InputVisitorBase
{
public:
    void visit(FileSource* fileSource);
    void visit(DirSource* dirSource);
    void visit(UDPSource* udpSource);
    void visit(UDPMulticastSource* udpSource);
    void visit(PassProcessor* passProcessor);
    void visit(RegexProcessor* regexProcessor);

    QJsonObject m_json;
};


class GetLabel : public QWidget, public InputVisitorBase
{
public:
    GetLabel();

    void visit(FileSource* fileSource);
    void visit(DirSource* dirSource);
    void visit(UDPSource* udpSource);
    void visit(UDPMulticastSource* udpSource);
    void visit(PassProcessor* passProcessor);
    void visit(RegexProcessor* regexProcessor);
};


class GetInputDialogWidget : public QWidget, public InputVisitorBase
{
public:
    GetInputDialogWidget();
    ~GetInputDialogWidget();

    void visit(FileSource* fileSource);
    void visit(DirSource* dirSource);
    void visit(UDPSource* udpSource);
    void visit(UDPMulticastSource* udpSource);
    void visit(PassProcessor* passProcessor);
    void visit(RegexProcessor* regexProcessor);

private:
    Ui::inputWidget* ui;
};


class GetDialog : public InputVisitorBase
{
public:
    GetDialog(AntiLog* antiLog);

    void visit(FileSource* fileSource);
    void visit(DirSource* dirSource);
    void visit(UDPSource* udpSource);
    void visit(UDPMulticastSource* udpSource);
    void visit(PassProcessor* passProcessor);
    void visit(RegexProcessor* regexProcessor);

private:
    AntiLog* m_antiLog;
};
