#include <stdio.h>
#include "../accessoryBase/accessoryBase.hpp"

void On_onSet(char **argv){
}

int main(int argc, char **argv){
    AccessoryBase accessoryBase(argv);

    accessoryBase.addCharacteristics(Characteristics("On", NULL, On_onSet));

    accessoryBase.run();

    return 0;
}