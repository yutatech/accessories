#ifndef ACCESSORYBASE_HPP_
#define ACCESORRYBASE_HPP_
#include <string>
#include <vector>

class Characteristics{
public:
    char *name;
    std::string (*onGet)(char **argv);
    void (*onSet)(char **argv);

    Characteristics(char *_name, std::string (*_onGet)(char **argv), void (*_onSet)(char **argv)) : 
        name(_name), onGet(_onGet), onSet(_onSet) {};
};

class AccessoryBase {
private:
    char **argv;
    std::string path;

    void onGet();
    void onSet();

    std::vector<Characteristics> charArray;

public:
    AccessoryBase(char **_argv);
    AccessoryBase(char **_argv, std::string _path);

    void run();
    void addCharacteristics(Characteristics characteristics);
};

#endif