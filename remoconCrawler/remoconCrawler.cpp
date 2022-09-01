#include <stdio.h>
#include "../remoconEmulator/remoconEmulator.hpp"
#include <chrono>
#include <thread>


int main(){
    rpi4_init();
    RemoconEmulator remocon(18);

    while(true){
        std::ifstream file;
        file.open("/home/pi/accessories/cmdBuffer", std::ios::out);

        std::string line_buffer;
        std::vector<std::string> data;
        while(std::getline(file, line_buffer)){
            if(line_buffer.size() == 0) break;
            data.push_back(line_buffer);
        }
        file.close();
        std::remove("/home/pi/accessories/cmdBuffer");

        for(const auto& e : data){
            remocon.push(e);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << e << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return 0;
}