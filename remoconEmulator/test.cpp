#include <stdio.h>
#include "remoconEmulator.hpp"
#include <string>

int main(int argc, char **argv) {

    RemoconEmulator remocon(18);

    std::string cmd = argv[1];
    remocon.push(cmd);


    return 0;
}