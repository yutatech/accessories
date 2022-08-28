#ifndef REMOCONEMULATOR_HPP_
#define REMOCONEMULATOR_HPP_
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class RemoconEmulator {
public:
    RemoconEmulator(int _gpio);
    void push(std::string csv_filename);
private:
    int gpio;
    //文字列のsplit機能
    std::vector<int> split(std::string str, char del) {
        int first = 0;
        int last = str.find_first_of(del);
        std::vector<int> result;
        while (first < str.size()) {
            std::string subStr(str, first, last - first);
            result.push_back(stoi(subStr));
            first = last + 1;
            last = str.find_first_of(del, first);
            if (last == std::string::npos) {
                last = str.size();
            }
        }
        return result;
    }

    std::vector<std::vector<int>> csv2vector(std::string filename, int ignore_line_num = 0){
        //csvファイルの読み込み
        std::ifstream reading_file;
        reading_file.open(filename, std::ios::in);
        if(!reading_file){
            std::vector<std::vector<int> > data;
            return data;
        }
        std::string reading_line_buffer;
        //最初のignore_line_num行を空読みする
        for(int line = 0; line < ignore_line_num; line++){
            getline(reading_file, reading_line_buffer);
            if(reading_file.eof()) break;
        }

        //二次元のvectorを作成
        std::vector<std::vector<int> > data;
        while(std::getline(reading_file, reading_line_buffer)){
            if(reading_line_buffer.size() == 0) break;
            std::vector<int> temp_data;
            temp_data = split(reading_line_buffer, ',');
            data.push_back(temp_data);
        }
        return data;
    }
};

#endif