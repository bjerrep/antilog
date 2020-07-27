#pragma once

class FileSource;
class DirSource;
class UDPSource;
class UDPMulticastSource;
class PassProcessor;
class RegexProcessor;

class InputVisitorBase
{
public:
    virtual void visit(FileSource* fileSource) = 0;
    virtual void visit(DirSource* fileSource) = 0;
    virtual void visit(UDPSource* udpSource) = 0;
    virtual void visit(UDPMulticastSource* udpSource) = 0;

    virtual void visit(PassProcessor* passProcessor) = 0;
    virtual void visit(RegexProcessor* regexProcessor) = 0;
};
