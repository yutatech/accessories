#include "remoconEmulator.hpp"

#include <unistd.h>

RemoconEmulator::RemoconEmulator(int _gpio) : gpio(_gpio), pwm(gpio, 38000) {
}

void RemoconEmulator::push(std::string csv_filename){
    std::vector<std::vector<int>> data = csv2vector(csv_filename);
    for(const auto& e : data){
        if(!e[0]){
            pwm = 0.5;
            usleep(e[1]);
        }
        else{
            pwm = 0;
            usleep(e[1]);
        }
    }
    pwm = 0;
}