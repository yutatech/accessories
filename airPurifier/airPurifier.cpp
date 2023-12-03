#include "../accessoryBase/accessoryBase.hpp"
#include <fstream>
#include <iostream>
#include <string.h>

std::string PATH = "/home/pi/accessories/airPurifier/";

void CurrentAirPurifierState_onGet(char **argv){
}

void RotationSpeed_onSet(char **argv){
}

void RotationSpeed_onGet(char **argv){
}

int main(int argc, char **argv){
    AccessoryBase airPurifier(argv, PATH);

    airPurifier.addCharacteristics(Characteristics("CurrentAirPurifierState", CurrentAirPurifierState_onGet, NULL));
    airPurifier.addCharacteristics(Characteristics("TargetAirPurifierState", NULL, NULL));
    airPurifier.addCharacteristics(Characteristics("RotationSpeed", RotationSpeed_onGet, RotationSpeed_onSet));

    airPurifier.run();

    return 0;
}