#include "../accessoryBase/accessoryBase.hpp"
#include <fstream>
#include <iostream>
#include <string>

std::string PATH = "/home/pi/accessories/airPurifier/";

std::string CurrentAirPurifierState_onGet(char **argv){
  return "0";
}

void RotationSpeed_onSet(char **argv){
}

std::string RotationSpeed_onGet(char **argv){
  return "10";
}

int main(int argc, char **argv){
    AccessoryBase airPurifier(argv, PATH);

    airPurifier.addCharacteristics(Characteristics("CurrentAirPurifierState", CurrentAirPurifierState_onGet, NULL));
    airPurifier.addCharacteristics(Characteristics("TargetAirPurifierState", NULL, NULL));
    airPurifier.addCharacteristics(Characteristics("RotationSpeed", RotationSpeed_onGet, RotationSpeed_onSet));

    airPurifier.run();

    return 0;
}