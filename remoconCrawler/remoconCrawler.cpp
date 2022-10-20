#include <stdio.h>
#include "../remoconEmulator/remoconEmulator.hpp"
#include <chrono>
#include <thread>


int main(){
    rpi4_init();
    RemoconEmulator remocon(18);

    while(true){
        std::ifstream ifs;
        ifs.open("/home/pi/accessories/cmdBuffer");

        std::string line_buffer;
        std::vector<std::string> data;
        while(std::getline(ifs, line_buffer)){
            if(line_buffer.size() == 0) break;
            data.push_back(line_buffer);
        }
        ifs.close();

        std::ofstream ofs;
        ofs.open("/home/pi/accessories/cmdBuffer", std::ios::trunc);
        ofs.close();

        for(const auto& e : data){
            remocon.push(e);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << e << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return 0;
}