#include "model.h"

#define LOGFILE_NAME "logfile.log"

int main(int argc, char* argv[]) {
    srand(time(nullptr));

    std::string filename = LOGFILE_NAME;
    if (argc >= 2) {
        filename = argv[1];
    }

    Model model(filename);
    model.startSimulation();

    return 0;
}
