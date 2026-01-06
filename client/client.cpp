//
// Created by Arjun Varma on 06/01/26.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#include "utils/SocketUtils.hpp"

constexpr const char* HOST = "127.0.0.1";
constexpr int PORT = 9100;
constexpr size_t BUFFER_SIZE = 4096;

std::string recvLine(int sockfd) {
    std::string line;
    char c;
    while (true) {
        int n = recv(sockfd, &c, 1, 0);
        if (n <= 0) break;
        if (c == '\n') break;
        line += c;
    }
    return line;
}

void listFiles(int sockfd) {
    send(sockfd, "list\n", 5, 0);

    int count = std::stoi(recvLine(sockfd));
    std::cout << "Listing " << count << " file(s):\n";

    for (int i = 0; i < count; i++) {
        std::cout << recvLine(sockfd) << "\n";
    }
}

void putFile(int sockfd, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "File not found: " << filename << "\n";
        return;
    }

    std::string cmd = "put\n";
    send(sockfd, cmd.c_str(), cmd.size(), 0);

    std::string fname = filename.substr(filename.find_last_of('/') + 1) + "\n";
    send(sockfd, fname.c_str(), fname.size(), 0);

    file.seekg(0, std::ios::end);
    long fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string sizeStr = std::to_string(fileSize) + "\n";
    send(sockfd, sizeStr.c_str(), sizeStr.size(), 0);

    char buffer[BUFFER_SIZE];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        send(sockfd, buffer, file.gcount(), 0);
    }

    int n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n > 0)
        std::cout << std::string(buffer, n) << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./client list | put <filename>\n";
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST, &serverAddr.sin_addr);

    if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    std::string command = argv[1];

    if (command == "list") {
        listFiles(sockfd);
    } else if (command == "put" && argc >= 3) {
        putFile(sockfd, argv[2]);
    } else {
        std::cerr << "Invalid command\n";
    }

    close(sockfd);
    return 0;
}
