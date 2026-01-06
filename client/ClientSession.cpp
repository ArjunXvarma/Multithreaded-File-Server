#include "ClientSession.hpp"
#include "utils/SocketUtils.hpp"

#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>

constexpr size_t BUFFER_SIZE = 4096;

ClientSession::ClientSession(int sock, std::string command)
    : sockfd(sock), command(command) {}

void ClientSession::run() {
    if (command == "list") {
        handleList();
    } else if (command.starts_with("put ")) {
        handlePut(command.substr(4));
    } else {
        std::cout << "Unknown command\n";
    }

    close(sockfd);
}

void ClientSession::handleList() {
    // Send command
    std::string cmd = "list\n";
    send(sockfd, cmd.c_str(), cmd.size(), 0);

    // Read count
    std::string countLine = SocketUtils::recvLine(sockfd);
    int count = std::stoi(countLine);

    std::cout << "Files on server:\n";
    for (int i = 0; i < count; i++) {
        std::cout << " - " << SocketUtils::recvLine(sockfd) << "\n";
    }
}

void ClientSession::handlePut(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "File not found: " << filename << "\n";
        return;
    }

    // Send command + metadata
    send(sockfd, "put\n", 4, 0);
    send(sockfd, (filename + "\n").c_str(), filename.size() + 1, 0);

    file.seekg(0, std::ios::end);
    long size = file.tellg();
    file.seekg(0);

    std::string sizeStr = std::to_string(size) + "\n";
    send(sockfd, sizeStr.c_str(), sizeStr.size(), 0);

    // Send file bytes
    char buffer[BUFFER_SIZE];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        send(sockfd, buffer, file.gcount(), 0);
    }

    // Read server response
    std::string response = SocketUtils::recvLine(sockfd);
    std::cout << response << "\n";
}
