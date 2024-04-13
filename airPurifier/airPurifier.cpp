#include "../accessoryBase/accessoryBase.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

std::string PATH = "/home/pi/accessories/airPurifier/";

/*
raspi -> esp
  tar_ap_off\n
  tar_ap_low\n
  tar_ap_middle\n
  tar_ap_high\n
  tar_light_on\n
  tar_light_off\n
  req_ap_stat\n
  req_light_stat\n

esp -> raspi
  cur_ap_off\n
  cur_ap_low\n
  cur_ap_middle\n
  cur_ap_high\n
  cur_light_on\n
  cur_light_off\n
*/

int ConnectToServer(){
    const char* hostname = "AirPurifier.local"; // サーバーのドメイン名
    const char* port = "85";             // ポート番号

    struct addrinfo hints, *serverInfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // ドメイン名を解決
    int status = getaddrinfo(hostname, port, &hints, &serverInfo);
    if (status != 0) {
        std::cerr << "Error in getaddrinfo: " << gai_strerror(status) << std::endl;
        return -1;
    }

    // ソケットの作成
    int clientSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        freeaddrinfo(serverInfo);
        return -1;
    }

    // サーバーに接続
    if (connect(clientSocket, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        freeaddrinfo(serverInfo);
        close(clientSocket);
        return -1;
    }
    freeaddrinfo(serverInfo);

    return clientSocket;
}

void SendToSever(int clientSocket, const char *data){
    // サーバーにデータを送信
    ssize_t bytesSent = send(clientSocket, data, strlen(data), 0);
    if (bytesSent == -1) {
        std::cerr << "Error sending data" << std::endl;
    }
}

int ReceiveFromServer(int clientSocket, char *buffer, int len){
    memset(buffer, 0, len);
    ssize_t bytesRead = recv(clientSocket, buffer, len, 0);
    return bytesRead;
}

void SendTargetValue(int value){
    if (value < 0 || value > 3)
      return;

    int clientSocket = ConnectToServer();

    if (clientSocket == -1)
      return;

    char dict[][20] = {
      "tar_ap_off\n",
      "tar_ap_low\n",
      "tar_ap_middle\n",
      "tar_ap_high\n"};

    SendToSever(clientSocket, dict[value]);

    close(clientSocket);
}

int ReadCurrentValue(){
    int clientSocket = ConnectToServer();
    if (clientSocket == -1)
      return -1;

    SendToSever(clientSocket, "req_ap_stat\n");

    char buffer[1024];
    int size = ReceiveFromServer(clientSocket, buffer, sizeof(buffer));

    close(clientSocket);

    if (strcmp(buffer, "cur_ap_off\n") == 0)
      return 0;
    else if (strcmp(buffer, "cur_ap_low\n") == 0)
      return 30;
    else if (strcmp(buffer, "cur_ap_middle\n") == 0)
      return 70;
    else if (strcmp(buffer, "cur_ap_high\n") == 0)
      return 100;

    return 0;
}

void Active_onSet(char **argv){
    if(strcmp(argv[4],"On") == 0){
        SendTargetValue(1);
    }
    else{
        SendTargetValue(0);
    }
}

std::string Active_onGet(char **argv){
    int speed = ReadCurrentValue();

    if (speed != 0)
      return "1";
    else
      return "0";
}

std::string CurrentAirPurifierState_onGet(char **argv){
    int speed = ReadCurrentValue();

    if (speed != 0)
      return "2";
    else
      return "0";
}

void RotationSpeed_onSet(char **argv){
    int speed = atoi(argv[4]);
    int stat_num = 0;

    if (speed == 0) {
        speed = 0;
        stat_num = 0;
    }
    else if (0 < speed && speed <= 30) {
        speed = 30;
        stat_num = 1;
    }
    else if (30 < speed && speed <= 70) {
        speed = 60;
        stat_num = 2;
    }
    else {
        speed = 100;
        stat_num = 3;
    }

    std::string speed_str = std::to_string(speed);
    strcpy(argv[4], speed_str.c_str());

    SendTargetValue(stat_num);
}

std::string RotationSpeed_onGet(char **argv){
    int speed = ReadCurrentValue();
    return std::to_string(speed);
}

int main(int argc, char **argv){
    AccessoryBase airPurifier(argv, PATH);

    airPurifier.addCharacteristics(Characteristics("Active", Active_onGet, Active_onSet));
    airPurifier.addCharacteristics(Characteristics("CurrentAirPurifierState", CurrentAirPurifierState_onGet, NULL));
    airPurifier.addCharacteristics(Characteristics("TargetAirPurifierState", NULL, NULL));
    airPurifier.addCharacteristics(Characteristics("RotationSpeed", RotationSpeed_onGet, RotationSpeed_onSet));

    airPurifier.run();

    return 0;
}