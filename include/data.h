#ifndef INCLUDE_DATA_H_
#define INCLUDE_DATA_H_

class Data {
private:
    unsigned _id;
    short _state;
    double _timeMoment;
public:
    Data(const unsigned id = 0, const short state = 0, const double timeMoment = 0) :
                        _id(id), _state(state), _timeMoment(timeMoment) {};
    unsigned getID() { return _id; };
    short getState() { return _state; };
    double getTime() { return _timeMoment; };
    void changeState(const short state) { _state = state; };
    void changeTime(const double newTime) { _timeMoment = newTime; };
};

#endif // INCLUDE_DATA_H_
