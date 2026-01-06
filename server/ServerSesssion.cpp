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

#include "metrics/metrics.hpp"
#include <chrono>

ServerSession::ServerSession(int sock, sockaddr_in clientAddr)
    : clientSock(sock) {

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
    clientIP = ip;
}

void ServerSession::run(std::atomic<bool>& shuttingDown) {
    SocketUtils::setReceiveTimeout(clientSock, 30);
    Metrics::connectionOpened();

    while (!shuttingDown.load()) {
        std::string command = SocketUtils::recvLine(clientSock);
        if (command.empty()) break;

        Metrics::requestStarted();
        auto start = std::chrono::steady_clock::now();

        try {
            if (command == "list") {
                FileService::listFiles(clientSock, clientIP);
            } else if (command == "put") {
                FileService::receiveFile(clientSock, clientIP);
            } else {
                Metrics::requestFailed();
                continue;
            }

            auto end = std::chrono::steady_clock::now();
            Metrics::requestSucceeded(
                std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            );
        } catch (...) {
            Metrics::requestFailed();
        }
    }

    Metrics::connectionClosed();
    close(clientSock);
}

void ServerSession::handleList() {
    FileService::listFiles(clientSock, clientIP);
}

void ServerSession::handlePut() {
    FileService::receiveFile(clientSock, clientIP);
}
