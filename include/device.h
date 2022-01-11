#ifndef INCLUDE_DEVICE_H
#define INCLUDE_DEVICE_H

#include "data.h"

class Device {
private:
    short _state;
    unsigned _minTimeToProcess;
    unsigned _maxTimeToProcess;
    unsigned _queueLength;
public:
    Device() {};
    Device(const short state, const unsigned minTime, const unsigned maxTime) :
            _state(state), _minTimeToProcess(minTime), _maxTimeToProcess(maxTime), _queueLength(0) {};
    short getState() { return _state; };
    unsigned getMinTimeToProcess() { return _minTimeToProcess; };
    unsigned getMaxTimeToProcess() { return _maxTimeToProcess; };
    unsigned getQueueLength() { return _queueLength; };
    void changeState(short state) { _state = state; };
    void correctQueueLength(int size) { _queueLength += size; };
};


#endif // INCLUDE_DEVICE_H
