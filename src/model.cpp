#include <cstdlib>

#include "model.h"

std::vector<std::string> logfileEntries = { "В момент времени {XXX} транзакт с идентификатором {XX} вошёл в модель",
                                            "В момент времени {XXX} транзакт с идентификатором {XX} встал в очередь {X}",
                                            "В момент времени {XXX} транзакт с идентификатором {XX} занял устройство {X}",
                                            "В момент времени {XXX} транзакт с идентификатором {XX} освободил устройство {X}",
                                            "В момент времени {XXX} транзакт с идентификатором {XX} вышел из модели" };

double generateTimeMoment(const unsigned minTime, const unsigned maxTime) {
    return rand() % (maxTime - minTime) + minTime + (double)(rand() % ACCURACY) / (double)ACCURACY;
}

std::string editRecord(const unsigned index, const double timeMoment, const unsigned id, const unsigned deviceID) {
    std::string record = logfileEntries[index];
    std::string editedTimeMoment = std::to_string(timeMoment);
    int i = (int)editedTimeMoment.size() - 1;
    while (editedTimeMoment[i] == '0') {
        i--;
    }

    i++;
    editedTimeMoment = editedTimeMoment.erase(i, editedTimeMoment.size() - i);

    record.replace(record.find("{XXX}"),5, editedTimeMoment);
    record.replace(record.find("{XX}"), 4, std::to_string(id));
    auto pos = record.find("{X}");
    if (pos != std::string::npos) {
        record.replace(pos, 3, std::to_string(deviceID));
    }

    return record;
}

void Model::initializeFields() {
    _file.open(_filename, std::ios_base::out);
    if (!_file.is_open()) {
        exit(-1);
    }

    _firstDevice = Device(EMPTY, MIN_PROCESS_TIME1, MAX_PROCESS_TIME1);
    _secondDevice = Device(EMPTY, MIN_PROCESS_TIME2, MAX_PROCESS_TIME2);
    _currentModelTime = 0;
    _lastModelTime = 0;
    _currentTransactID = 1;
}

Model::Model() {
    _filename = STANDARD_LOGFILE_NAME;
    initializeFields();
}

Model::Model(std::string filename) {
    _filename = filename;
    initializeFields();
}

void Model::addRecordToFile(std::string record) {
    _file << record << std::endl;
}

void Model::startSimulation() {
    _fec.clear();
    _cec.clear();
    _fec.push_back(Data(0, EXIT, MODEL_TIME));

    bool isTimeFull = false;
    while(!isTimeFull) {
        executePhaseInput();
        executePhaseTimerCorrection();
        if (executePhaseViewing()) {
            isTimeFull = true;
        }
    }
}

void Model::executePhaseInput() {
    if (_currentModelTime == _lastModelTime) {
        double timeMoment = _lastModelTime + generateTimeMoment(0, MAX_TIME_TO_COME);
        _lastModelTime = timeMoment;
        _fec.push_back(Data(_currentTransactID++, WAIT_TO_BE_ADDED, timeMoment));
    }
}

void Model::executePhaseTimerCorrection() {
    double minTime = MODEL_TIME;
    for (auto item: _fec) {
        if (item.getTime() < minTime) {
            minTime = item.getTime();
        }
    }

    _currentModelTime = minTime;

    for (int i = 0; i < _fec.size(); ++i) {
        Data item = _fec[i];
        if (item.getTime() == minTime) {
            _cec.push_back(item);
            _fec.erase(_fec.begin() + i);
        }
    }
}

bool Model::executePhaseViewing() {
    unsigned i = 0;

    for (auto item: _cec) {
        if (item.getState() == EXIT) {
            return true;
        } else if (item.getState() == WAIT_TO_BE_ADDED) {
            addRecordToFile(editRecord(ENTERED, item.getTime(), item.getID(), 0));
            if (_firstDevice.getState() == EMPTY) {
                addRecordToFile(editRecord(OCCUPIED_DEVICE, item.getTime(), item.getID(), 1));
                _firstDevice.changeState(BUSY);
                item.changeTime(_currentModelTime + generateTimeMoment(_firstDevice.getMinTimeToProcess(), _firstDevice.getMaxTimeToProcess()));
                item.changeState(IN_PROCESS1);

                _fec.push_back(item);
                _cec.erase(_cec.begin() + i);
            } else if (_secondDevice.getState() == EMPTY) {
                addRecordToFile(editRecord(OCCUPIED_DEVICE, item.getTime(), item.getID(), 2));
                _secondDevice.changeState(BUSY);
                item.changeTime(_currentModelTime + generateTimeMoment(_secondDevice.getMinTimeToProcess(), _secondDevice.getMaxTimeToProcess()));
                item.changeState(IN_PROCESS2);

                _fec.push_back(item);
                _cec.erase(_cec.begin() + i);
            } else if (_firstDevice.getQueueLength() <= _secondDevice.getQueueLength()) {
                addRecordToFile(editRecord(GET_IN_LINE, item.getTime(), item.getID(), 1));
                _firstDevice.correctQueueLength(1);
                _cec[i].changeState(WAIT_QUEUE1);
                i++;
            } else {
                addRecordToFile(editRecord(GET_IN_LINE, item.getTime(), item.getID(), 2));
                _secondDevice.correctQueueLength(1);
                _cec[i].changeState(WAIT_QUEUE2);
                i++;
            }
        } else if (item.getState() == IN_PROCESS1) {
            _firstDevice.changeState(EMPTY);
            addRecordToFile(editRecord(RELEASED_DEVICE, _currentModelTime, item.getID(), 1));
            addRecordToFile(editRecord(EXITED, _currentModelTime, item.getID(), 0));
            _cec.erase(_cec.begin() + i);
            unsigned nextTransact = 0;
            double minTime = MODEL_TIME;
            for (int j = 0; j < _cec.size(); ++j) {
                if (_cec[j].getState() == WAIT_QUEUE1) {
                    if (minTime > _cec[j].getTime()) {
                        minTime = _cec[j].getTime();
                        nextTransact = j;
                    }
                }
            }

            if (minTime != MODEL_TIME) {
                _firstDevice.correctQueueLength(-1);
                _firstDevice.changeState(BUSY);
                Data& tempTransact = _cec[nextTransact];
                tempTransact.changeState(IN_PROCESS1);
                tempTransact.changeTime(_currentModelTime + generateTimeMoment(_firstDevice.getMinTimeToProcess(), _firstDevice.getMaxTimeToProcess()));
                addRecordToFile(editRecord(OCCUPIED_DEVICE, _currentModelTime, tempTransact.getID(), 1));
                _fec.push_back(tempTransact);
                _cec.erase(_cec.begin() + nextTransact);
            }
        } else if (item.getState() == IN_PROCESS2) {
            _secondDevice.changeState(EMPTY);
            addRecordToFile(editRecord(RELEASED_DEVICE, _currentModelTime, item.getID(), 2));
            addRecordToFile(editRecord(EXITED, _currentModelTime, item.getID(), 0));
            _cec.erase(_cec.begin() + i);
            unsigned nextTransact = 0;
            double minTime = MODEL_TIME;
            for (int j = 0; j < _cec.size(); ++j) {
                if (_cec[j].getState() == WAIT_QUEUE2) {
                    if (minTime > _cec[j].getTime()) {
                        minTime = _cec[j].getTime();
                        nextTransact = j;
                    }
                }
            }

            if (minTime != MODEL_TIME) {
                _secondDevice.correctQueueLength(-1);
                _secondDevice.changeState(BUSY);
                Data& tempTransact = _cec[nextTransact];
                tempTransact.changeState(IN_PROCESS2);
                tempTransact.changeTime(_currentModelTime + generateTimeMoment(_secondDevice.getMinTimeToProcess(), _secondDevice.getMaxTimeToProcess()));
                addRecordToFile(editRecord(OCCUPIED_DEVICE, _currentModelTime, tempTransact.getID(), 2));
                _fec.push_back(tempTransact);
                _cec.erase(_cec.begin() + nextTransact);
            }
        } else {
            i++;
        }
    }

    return false;
}

Model::~Model() {
    _file.close();
}
