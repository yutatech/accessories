#include "accessoryBase.hpp"
#include <string.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <sstream>

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


std::string AccessoryBase::readFile(const std::string file_name) {
    std::ifstream file;
    file.open(file_name);
    while (!file) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        file.open(file_name);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void AccessoryBase::writeFile(const std::string file_name, const std::string content) {
    std::ofstream file;
    file.open(file_name);
    while (!file){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        file.open(file_name);
    }
    file << content;
    file.close();
}

void AccessoryBase::onGet(){
    for(const auto& e : charArray){
        if(strcmp(argv[3], e.name) == 0){
            std::string file_name = path + std::string(e.name) + ".conf";

            if(e.onGet != NULL) {
                std::string value = e.onGet(argv, readFile(file_name));
                writeFile(file_name, value);
            }
            std::cout << readFile(file_name) << std::endl;
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