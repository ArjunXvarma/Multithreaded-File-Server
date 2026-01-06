//
// Created by Arjun Varma on 06/01/26.
//

#include "FileService.hpp"

#include "FileService.hpp"
#include "utils/Logger.hpp"
#include "utils/SocketUtils.hpp"

#include <filesystem>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sys/socket.h>

constexpr size_t BUFFER_SIZE = 4096;

void FileService::listFiles(int clientSock, const std::string& clientIP) {
    std::vector<std::string> files;

    if (std::filesystem::exists("serverFiles")) {
        for (const auto& entry : std::filesystem::directory_iterator("serverFiles")) {
            if (entry.is_regular_file())
                files.push_back(entry.path().filename());
        }
    }

    sendLine(clientSock, std::to_string(files.size()));

    for (const auto& file : files) {
        sendLine(clientSock, file);
    }

    Logger::log(clientIP, "list");
}

void FileService::receiveFile(int clientSock, const std::string& clientIP) {
    std::string fileName = SocketUtils::recvLine(clientSock);
    std::string sizeLine = SocketUtils::recvLine(clientSock);

    if (fileName.empty() || sizeLine.empty()) {
        Logger::log(clientIP, "put_failed");
        return;
    }

    long fileSize = std::stol(sizeLine);
    std::filesystem::create_directory("serverFiles");
    std::string filePath = "serverFiles/" + fileName;

    if (std::filesystem::exists(filePath)) {
        sendLine(clientSock, "Error: File already exists.");
        Logger::log(clientIP, "put_exists");
        return;
    }

    std::ofstream out(filePath, std::ios::binary);
    char buffer[BUFFER_SIZE];
    long remaining = fileSize;
    bool transferFailed = false;

    while (remaining > 0) {
        int n = recv(clientSock,
                     buffer,
                     std::min(BUFFER_SIZE, (size_t)remaining),
                     0);

        if (n <= 0) {
            transferFailed = true;
            break;
        }

        out.write(buffer, n);
        remaining -= n;
    }

    out.close();

    if (transferFailed || remaining > 0) {
        std::filesystem::remove(filePath);
        sendLine(clientSock, "Error: Transfer failed. File discarded.");
        Logger::log(clientIP, "put_failed");
        return;
    }

    sendLine(clientSock, "Uploaded file " + fileName);
    Logger::log(clientIP, "put");
}

void FileService::sendLine(int sockfd, const std::string& msg) {
    std::string line = msg + "\n";
    send(sockfd, line.c_str(), line.size(), 0);
}

