#ifndef INCLUDE_MODEL_H
#define INCLUDE_MODEL_H

#include <fstream>
#include <string>
#include <vector>

#include "device.h"

#define STANDARD_LOGFILE_NAME "log.l"

#define MODEL_TIME 3600

#define ACCURACY 1000

#define MAX_TIME_TO_COME 25

#define MIN_PROCESS_TIME1 11
#define MAX_PROCESS_TIME1 25

#define MIN_PROCESS_TIME2 9
#define MAX_PROCESS_TIME2 25

// For Devices
#define EMPTY 1
#define BUSY 2

// For Transacts
#define WAIT_TO_BE_ADDED 1
#define WAIT_QUEUE1 2
#define WAIT_QUEUE2 3
#define IN_PROCESS1 4
#define IN_PROCESS2 5
#define EXIT 6

#define ENTERED 0
#define GET_IN_LINE 1
#define OCCUPIED_DEVICE 2
#define RELEASED_DEVICE 3
#define EXITED 4

class Model {
private:
    Device _firstDevice;
    Device _secondDevice;
    std::vector<Data> _cec;
    std::vector<Data> _fec;
    std::string _filename;
    std::fstream _file;
    double _currentModelTime;
    double _lastModelTime;
    unsigned _currentTransactID;

    void initializeFields();
    void addRecordToFile(std::string record);
    void executePhaseInput();
    void executePhaseTimerCorrection();
    bool executePhaseViewing();
public:
    Model();
    Model(std::string filename);
    void startSimulation();
    ~Model();
};

#endif // INCLUDE_MODEL_H
