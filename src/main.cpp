#include "model.h"

#define LOGFILE_NAME "logfile.log"

int main() {
    srand(time(nullptr));
    Model model(LOGFILE_NAME);
    model.startSimulation();

    return 0;
}
