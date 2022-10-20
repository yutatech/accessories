#include "../accessoryBase/accessoryBase.hpp"
#include <fstream>
#include <iostream>
#include <string.h>

std::string PATH = "/home/pi/accessories/light/";

void push(std::string cmdFileName){
    std::ofstream ofs("/home/pi/accessories/cmdBuffer", std::ios::app);
    ofs << cmdFileName << std::endl;
}

void On_onSet(char **argv){
    std::ifstream on(PATH + "On.conf");
    std::string on_str;
    std::getline(on, on_str);

    std::ifstream brightness(PATH + "Brightness.conf");
    std::string brightness_str;
    std::getline(brightness, brightness_str);
    brightness.close();

    if(on_str != argv[4]){
        if(strcmp(argv[4],"1") == 0){
            if(atoi(brightness_str.c_str()) == 0){
                std::ofstream brightness(PATH + "Brightness.conf");
                brightness << "100";
                push(PATH + "cmd_zento.csv");
            }
        }
        else{
            std::ofstream brightness(PATH + "Brightness.conf");
            brightness << "0";
            push(PATH + "cmd_shoto.csv");
        }
    }
}

void adjust_brightness(int curBrightness, int tarBrightness){
    if(curBrightness > 96) curBrightness = 96;
    if(tarBrightness > 96) tarBrightness = 96;

    curBrightness = (curBrightness - 17) /16;
    tarBrightness = (tarBrightness - 17) /16;

    for(; tarBrightness > curBrightness; curBrightness++)
        push(PATH + "cmd_mei.csv");

    for(; tarBrightness < curBrightness; curBrightness--)
        push(PATH + "cmd_an.csv");

}

void Brightness_onSet(char **argv){
    int brightness = atoi(argv[4]);
    std::string brightness_str = argv[4];

    std::fstream file(PATH + "Brightness.conf", std::ios::in);
    std::string str;
    std::getline(file, str);
    file.close();
    int curBrightness = atoi(str.c_str());

    file.open(PATH + "Brightness.conf", std::ios::trunc | std::ios::out);

    printf("%d\n", brightness);

    if(brightness == 0){
        if(curBrightness > 0){
            push(PATH + "cmd_shoto.csv");
            file << "0";
        }
    }
    else if(1 <= brightness && brightness < 17){
        if(!(1 <= curBrightness && curBrightness < 17))
            push(PATH + "cmd_joyato.csv");
        file << brightness_str;
    }
    else if(17 <= brightness && brightness < 33){
        if(curBrightness < 17){
            push(PATH + "cmd_zento.csv");
            curBrightness = 96;
            adjust_brightness(curBrightness, brightness);
        }
        adjust_brightness(curBrightness, brightness);
        file << brightness_str;
    }
    else if(33 <= brightness && brightness < 49){
        if(curBrightness < 17){
            push(PATH + "cmd_zento.csv");
            curBrightness = 96;
            adjust_brightness(curBrightness, brightness);
            return;
        }
        adjust_brightness(curBrightness, brightness);
        file << brightness_str;
    }
    else if(49 <= brightness && brightness < 65){
        if(curBrightness < 17){
            push(PATH + "cmd_zento.csv");
            curBrightness = 96;
            adjust_brightness(curBrightness, brightness);
            return;
        }
        adjust_brightness(curBrightness, brightness);
        file << brightness_str;
    }
    else if(65 <= brightness && brightness < 81){
        if(curBrightness < 17){
            push(PATH + "cmd_zento.csv");
            curBrightness = 96;
            adjust_brightness(curBrightness, brightness);
            return;
        }
        adjust_brightness(curBrightness, brightness);
        file << brightness_str;
    }
    else if(81 <= brightness && brightness <= 100){
        if(curBrightness < 81){
            push(PATH + "cmd_zento.csv");
        }
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