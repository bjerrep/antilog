#include "antilogapi.h"
#include "antilog.h"

#include <QHostAddress>


AntiLogApi::AntiLogApi(AntiLog* antilog)
  : m_antiLog(antilog)
{
}

UdpSourceVector AntiLogApi::getUdpSources()
{
    UdpSourceVector udpSources;
    for(int i=0; i<m_antiLog->m_inputList.count(); i++)
    {
        UDPSource* source = dynamic_cast<UDPSource*>(m_antiLog->m_inputList.getInput(i)->getSourceEntry());
        if (source)
        {
            udpSources.push_back(source);
        }
    }
    return udpSources;
}

InputVector AntiLogApi::getInputs()
{
    return m_antiLog->m_inputList.getAllInputs();
}

void AntiLogApi::addInput(Input* input)
{
    m_antiLog->m_inputList.appendAndConnect(input);
}

void AntiLogApi::deleteInput(Input* input)
{
    int i = 0;
    for(auto _input : getInputs())
    {
        if (input == _input)
        {
            m_antiLog->m_inputList.removeAt(i);
            return;
        }
        ++i;
    }
}

