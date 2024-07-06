#include "../accessoryBase/accessoryBase.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include <chrono>
#include <thread>

std::string PATH = "/home/pi/accessories/thermostat/";

void push(std::string cmdFileName){
    std::ofstream ofs("/home/pi/accessories/cmdBuffer", std::ios::app);
    ofs << cmdFileName << std::endl;
    ofs << cmdFileName << std::endl;
}

void TargetHeatingCoolingState_onSet(char **argv){
    std::ifstream ofs(PATH+"TargetTemperature.conf", std::ios::in);
    std::string temp_str;
    std::getline(ofs, temp_str);
    int temp = atoi(temp_str.c_str());
    temp_str = std::to_string(temp);
    if(strcmp(argv[4],"0") == 0){
        push(PATH + "cmd_off_cool_18.csv");
    }
    else if(strcmp(argv[4],"1") == 0){
        push(PATH + "cmd_on_heat_" + temp_str + ".csv");
    }
    else if(strcmp(argv[4],"2") == 0){
        push(PATH + "cmd_on_cool_" + temp_str + ".csv");
    }
    else{
        strcpy(argv[4], "0");
        push(PATH + "cmd_off_cool_18.csv");
    }

    // 冷暖房時は除湿機能を強制的にOFFにする
    std::ofstream file;
    file.open(PATH + std::string("Active") + ".conf");
    while (!file){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        file.open(PATH + std::string("Active") + ".conf");
    }
    file << 0;
}


void TargetTemperature_onSet(char **argv){
    std::ifstream ofs(PATH + "TargetHeatingCoolingState.conf", std::ios::in);
    std::string state_str;
    std::getline(ofs, state_str);

    int temp = atoi(argv[4]);

    if(state_str == "1"){
        if(temp > 30)
            temp = 30;
        else if(temp < 14)
            temp = 14;
        state_str = "heat";
    }
    else if(state_str == "2"){
        if(temp > 32)
            temp = 32;
        else if(temp < 18)
            temp = 18;
        state_str = "cool";
    }
    else{
        return;
    }

    std::string temp_str = std::to_string(temp);

    strcpy(argv[4], temp_str.c_str());

    push(PATH + "cmd_on_" + state_str + "_" + temp_str + ".csv");
}

// 除湿時は冷暖房機能を強制的にOFFにする
void HumidifierDehumidifier_onSet(char **argv){
    strcpy(argv[4], std::to_string(2).c_str()); // 強制的にDehumidifierにする
    push(PATH + "cmd_on_dehumidification.csv");
    printf("abcdef");

    std::ofstream file;
    file.open(PATH + std::string("TargetHeatingCoolingState") + ".conf");
    while (!file){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        file.open(PATH + std::string("TargetHeatingCoolingState") + ".conf");
    }
    file << 0;
}


void Active_onSet(char **argv){
    if (atoi(argv[4]) == 0) {
        push(PATH + "cmd_off_cool_18.csv");
    }
}

int main(int argc, char **argv){
    AccessoryBase thermostat(argv, PATH);

    thermostat.addCharacteristics(Characteristics("TargetHeatingCoolingState", NULL, TargetHeatingCoolingState_onSet));
    thermostat.addCharacteristics(Characteristics("TargetTemperature", NULL, TargetTemperature_onSet));
    thermostat.addCharacteristics(Characteristics("TargetHumidifierDehumidifierState", NULL, HumidifierDehumidifier_onSet));
    thermostat.addCharacteristics(Characteristics("Active", NULL, Active_onSet));

    thermostat.run();

    if(strcmp(argv[3], "CurrentTemperature") == 0)
        printf("25");

    if(strcmp(argv[3], "CurrentHeatingCoolingState") == 0){
        std::ifstream ofs(PATH + "TargetHeatingCoolingState.conf");
        std::string state_str;
        std::getline(ofs, state_str);
        printf(state_str.c_str());
    }

    if(strcmp(argv[3], "CurrentHumidifierDehumidifierState") == 0){
        std::ifstream ofs(PATH + "TargetHumidifierDehumidifierState.conf");
        std::string state_str;
        std::getline(ofs, state_str);
        printf(state_str.c_str());
    }

    return 0;
}
