#include "../accessoryBase/accessoryBase.hpp"
#include <fstream>
#include <iostream>
#include <string.h>

std::string PATH = "/home/pi/accessories/light/";

void push(std::string cmdFileName){
    std::ofstream ofs("/home/pi/accessories/cmdBuffer", std::ios::app);
    ofs << cmdFileName << std::endl;
    std::cout << cmdFileName << std::endl;
}

void On_onSet(char **argv){
    if(strcmp(argv[4],"1") == 0){
        std::ofstream file(PATH + "Brightness.conf");
        file << "100";
        push(PATH + "cmd_zento.csv");
    }
    else{
        std::ofstream file(PATH + "Brightness.conf");
        file << "0";
        push(PATH + "cmd_shoto.csv");
    }
    printf("きた\n");
}

void Brightness_onSet(char **argv){
    int brightness = atoi(argv[4]);
    std::string brightness_str = argv[4];
    printf("%d\n", brightness);

    std::string str;

    std::fstream file(PATH + "Brightness.conf");
    std::getline(file, str);
    int curBrightness = atoi(str.c_str());

    if(0 <= brightness && brightness < 14){
        if(curBrightness >= 14){
            push(PATH + "cmd_shoto.csv");
            file << "0";
            std::ofstream on(PATH + "On.conf");
            on << "0";
        }
    }
    else if(14 <= brightness && brightness < 28){
        if(!(14 <= curBrightness && curBrightness < 28))
            push(PATH + "cmd_joyato.csv");
        file << brightness_str;
    }
    else if(28 <= brightness && brightness < 42){
        if(curBrightness < 28){
            push(PATH + "cmd_zento.csv");
            file << "100";
            return;
        }
        else if(curBrightness >= 42)
            push(PATH + "cmd_an.csv");
        file << brightness_str;
    }
    else if(42 <= brightness && brightness < 56){
        if(curBrightness < 28){
            push(PATH + "cmd_zento.csv");
            file << "100";
            return;
        }
        else if(curBrightness < 42)
            push(PATH + "cmd_mei.csv");
        else if(56 <= curBrightness)
            push(PATH + "cmd_an.csv");
        file << brightness_str;
    }
    else if(56 <= brightness && brightness < 70){
        if(curBrightness < 28){
            push(PATH + "cmd_zento.csv");
            file << "100";
            return;
        }
        else if(curBrightness < 56)
            push(PATH + "cmd_mei.csv");
        else if(70 <= curBrightness)
            push(PATH + "cmd_an.csv");
        file << brightness_str;
    }
    else if(70 <= brightness && brightness < 84){
        if(curBrightness < 28){
            push(PATH + "cmd_zento.csv");
            file << "100";
            return;
        }
        else if(curBrightness < 70)
            push(PATH + "cmd_mei.csv");
        else if(84 <= curBrightness)
            push(PATH + "cmd_an.csv");
        file << brightness_str;
    }
    else if(84 <= brightness && brightness <= 100){
        if(curBrightness < 28){
            push(PATH + "cmd_zento.csv");
            file << "100";
            return;
        }
        else if(curBrightness < 70)
            push(PATH + "cmd_mei.csv");
        else if(84 <= curBrightness)
            push(PATH + "cmd_an.csv");
        file << brightness_str;
    }
}

int main(int argc, char **argv){
    AccessoryBase light(argv, PATH);

    light.addCharacteristics(Characteristics("On", NULL, On_onSet));
    light.addCharacteristics(Characteristics("Brightness", NULL, Brightness_onSet));

    light.run();

    return 0;
}