#include "accessoryBase.hpp"
#include <string.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

AccessoryBase::AccessoryBase(char **_argv) : argv(_argv), path(""), charArray({}) {
}

AccessoryBase::AccessoryBase(char **_argv, std::string _path) : argv(_argv), path(_path), charArray({}) {
}

void AccessoryBase::addCharacteristics(Characteristics characteristics) {
    charArray.push_back(characteristics);
}

void AccessoryBase::run(){
    if(strcmp(argv[1],"Get") == 0)
        onGet();
    else
        onSet();
}

void AccessoryBase::onGet(){
    for(const auto& e : charArray){
        if(strcmp(argv[3], e.name) == 0){

            if(e.onGet != NULL) {
                std::string value = e.onGet(argv);
                std::ofstream file;
                file.open(path + std::string(e.name) + ".conf");
                while (!file){
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    file.open(path + std::string(e.name) + ".conf");
                }
                file << value;
                file.close();
            }
            std::ifstream file;
            file.open(path + std::string(e.name) + ".conf");
            while(!file){
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                file.open(path + std::string(e.name) + ".conf");
            }
            std::cout << file.rdbuf() << std::endl;
        }
    }
}

void AccessoryBase::onSet(){
    for(const auto& e : charArray){
        if(strcmp(argv[3], e.name) == 0){
            if(e.onSet != NULL){
                e.onSet(argv);
            }
            std::ofstream file;
            file.open(path + std::string(e.name) + ".conf");
            while (!file){
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                file.open(path + std::string(e.name) + ".conf");
            }
            file << argv[4];
        }
    }
}