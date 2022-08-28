#include "remoconEmulator.hpp"

#include <pigpio.h>
#include <unistd.h>

RemoconEmulator::RemoconEmulator(int _gpio) : gpio(_gpio) {
    while(true){
        if(gpioInitialise() >= 0)
            break;
        usleep(5000000);
    }
    gpioSetMode(gpio, PI_OUTPUT);
}

void RemoconEmulator::push(std::string csv_filename){
    std::vector<std::vector<int>> data = csv2vector(csv_filename);
    for(const auto& e : data){
        if(!e[0]){
            gpioHardwarePWM(gpio, 38000, 500000);
            usleep(e[1]-140);
        }
        else{
            gpioHardwarePWM(gpio, 38000, 0);
            usleep(e[1]);
        }
    }
    std::cout << "  >> " << csv_filename << "  " << data.size() << std::endl;
    gpioWrite(gpio, 0);
}