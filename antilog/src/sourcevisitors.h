#pragma once

#include "inputsources.h"
#include "inputprocessors.h"
#include "ui_inputwidget.h"
#include "inputvisitorbase.h"

#include <QJsonObject>


class GetName : public InputVisitorBase
{
public:
    void visit(FileSource* fileSource);
    void visit(DirSource* dirSource);
    void visit(UDPSource* udpSource);
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
    void visit(PassProcessor* passProcessor);
    void visit(RegexProcessor* regexProcessor);

private:
    Ui::inputWidget* ui;
};


class GetDialog : public QWidget, public InputVisitorBase
{
public:
    GetDialog();

    void visit(FileSource* fileSource);
    void visit(DirSource* dirSource);
    void visit(UDPSource* udpSource);
    void visit(PassProcessor* passProcessor);
    void visit(RegexProcessor* regexProcessor);

private:
    Ui::inputWidget* ui;
};
