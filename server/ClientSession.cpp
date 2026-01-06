//
// Created by Arjun Varma on 06/01/26.
//

#include "ClientSession.hpp"

#include "ClientSession.hpp"
#include "utils/SocketUtils.hpp"
#include "FileService.hpp"

#include <arpa/inet.h>
#include <unistd.h>

ClientSession::ClientSession(int sock, sockaddr_in addr)
    : clientSock(sock) {
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
    clientIP = ip;
}

void ClientSession::run(std::atomic<bool>& shuttingDown) {
    SocketUtils::setReceiveTimeout(clientSock, 30);

    while (!shuttingDown.load()) {
        std::string command = SocketUtils::recvLine(clientSock);
        if (command.empty()) break;

        if (command == "list") {
            FileService::listFiles(clientSock, clientIP);
        } else if (command == "put") {
            FileService::receiveFile(clientSock, clientIP);
        }
    }

    close(clientSock);
}
