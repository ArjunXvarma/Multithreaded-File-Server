//
// Created by Arjun Varma on 06/01/26.
//

#include "ServerSesssion.hpp"

#include "FileService.hpp"
#include "utils/SocketUtils.hpp"
#include "utils/Logger.hpp"

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

ServerSession::ServerSession(int sock, sockaddr_in clientAddr)
    : clientSock(sock) {

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
    clientIP = ip;
}

void ServerSession::run(std::atomic<bool>& shuttingDown) {
    // Idle timeout for safety
    SocketUtils::setReceiveTimeout(clientSock, 30);

    while (!shuttingDown.load()) {
        std::string command = SocketUtils::recvLine(clientSock);

        if (command.empty()) {
            // client disconnected or timeout
            break;
        }

        if (command == "list") {
            handleList();
        } else if (command == "put") {
            handlePut();
        } else {
            // Unknown command — ignore or log
            Logger::log(clientIP, "unknown_command");
        }
    }

    close(clientSock);
}

void ServerSession::handleList() {
    FileService::listFiles(clientSock, clientIP);
}

void ServerSession::handlePut() {
    FileService::receiveFile(clientSock, clientIP);
}
