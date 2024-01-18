#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctime>
#include "include/solve.h"

template<typename ...Args>
void log(std::ofstream &log_file, const Args &...log_infos) {
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);
    char time[80];
    std::strftime(time, sizeof(time), "[%Y-%m-%d %H:%M:%S]: ", localTime);
    log_file << time;
    ((log_file << log_infos << ' '), ...);
    log_file << std::endl;
}


int main(int argc, char *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address{};
    int addrlen = sizeof(address);
    char reqest[1024] = {0};
    char response[65536] = {0};

    std::ofstream info_log(argv[2], std::ios::app); //信息日志
    std::ofstream error_log(argv[3], std::ios::app); //错误日志


    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        log(error_log, "socket failed");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(std::stoi(argv[1]));
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        log(error_log, "bind failed");
        return 1;
    }

    if (listen(server_fd, 3) == -1) {
        log(error_log, "listen failed");
        return 1;
    }
    while (true) {
        std::memset(reqest, 0, sizeof(reqest));
        std::memset(response, 0, sizeof(response));
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) == -1) {
            log(error_log, "accept failed");
            return 1;
        }
        size_t bytesRead = recv(new_socket, reqest, sizeof(reqest), 0);
        if (bytesRead == -1) {
            log(error_log, "recv failed");
        } else if (bytesRead == 0) {
            log(error_log, "connect closed");
        }
        int success = 1;
        try {
            solve(reqest, response);
        } catch (...) {
            success = 0;
            strcpy(response, "不合法的输入！");
        }
        send(new_socket, response, strlen(response), 0);
        if (success) {
            log(info_log, "Successfully solve request:", reqest);
        } else {
            log(info_log, "Failed with invalid request:", reqest);
        }
    }
    return 0;
}
