//
// Created by Arjun Varma on 06/01/26.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <filesystem>
#include <atomic>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

constexpr int PORT = 9100;
constexpr size_t BUFFER_SIZE = 4096;

std::mutex logMutex;

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

void logRequest(const std::string& clientIP, const std::string& request) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::ofstream log("log.txt", std::ios::app);
    time_t now = time(nullptr);
    log << std::ctime(&now) << " | " << clientIP << " | " << request << "\n";
}

void listFiles(int clientSock, const std::string& clientIP) {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator("serverFiles")) {
        if (entry.is_regular_file())
            files.push_back(entry.path().filename());
    }

    std::string count = std::to_string(files.size()) + "\n";
    send(clientSock, count.c_str(), count.size(), 0);

    for (const auto& file : files) {
        std::string line = file + "\n";
        send(clientSock, line.c_str(), line.size(), 0);
    }

    logRequest(clientIP, "list");
}

void receiveFile(int clientSock, const std::string& clientIP) {
    std::string fileName = recvLine(clientSock);
    long fileSize = std::stol(recvLine(clientSock));

    std::filesystem::create_directory("serverFiles");
    std::string filePath = "serverFiles/" + fileName;

    if (std::filesystem::exists(filePath)) {
        std::string err = "Error: File already exists.\n";
        send(clientSock, err.c_str(), err.size(), 0);
        logRequest(clientIP, "put");
        return;
    }

    std::ofstream out(filePath, std::ios::binary);
    char buffer[BUFFER_SIZE];
    long remaining = fileSize;

    while (remaining > 0) {
        int n = recv(clientSock, buffer,
                     std::min(BUFFER_SIZE, static_cast<size_t>(remaining)), 0);
        if (n <= 0) break;

        out.write(buffer, n);
        remaining -= n;
    }

    std::string ok = "Uploaded file " + fileName + "\n";
    send(clientSock, ok.c_str(), ok.size(), 0);
    logRequest(clientIP, "put");
}

void handleClient(int clientSock, sockaddr_in clientAddr) {
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
    std::string clientIP(ip);

    while (true) {
        std::string command = recvLine(clientSock);
        if (command.empty()) break;

        if (command == "list") {
            listFiles(clientSock, clientIP);
        } else if (command == "put") {
            receiveFile(clientSock, clientIP);
        }
    }

    close(clientSock);
}

int main() {
    std::ofstream("log.txt", std::ios::trunc); // reset log

    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSock, 20);

    std::cout << "Server started. Listening on port " << PORT << "\n";

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);
        int clientSock = accept(serverSock, (sockaddr*)&clientAddr, &len);
        if (clientSock < 0) continue;

        std::thread(handleClient, clientSock, clientAddr).detach();
    }

    close(serverSock);
    return 0;
}
