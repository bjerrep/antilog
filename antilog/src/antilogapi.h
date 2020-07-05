#pragma once

#include "antilog.h"
#include "inputs/inputsources.h"
#include "inputs/inputprocessors.h"
#include "inputs/input.h"

#include <QString>
#include <QVector>

using UdpSourceVector = QVector<UDPSource*>;
using InputVector = QList<Input*>;


class AntiLogApi
{
public:
    AntiLogApi(AntiLog* antilog);

    // Get a vector of all UDPSources if any. The returned UDPSources can be modified directly.
    UdpSourceVector getUdpSources();

    InputVector getInputs();

    // Add an externally constructed input. Takes ownership.
    void addInput(Input* input);

    // Notice that the Input pointer will be invalid after this call since it
    // actually is what is deleted from the model. If working on a previously retrieved
    // InputVector this will now have an invalid entry for this Input so dont
    // iterate over that InputVector more than once.
    //
    void deleteInput(Input* input);

private:
    AntiLog* m_antiLog;
};

