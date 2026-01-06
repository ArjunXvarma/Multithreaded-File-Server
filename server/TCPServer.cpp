//
// Created by Arjun Varma on 06/01/26.
//

#include "TCPServer.hpp"
#include "ClientSession.hpp"
#include "utils/Logger.hpp"

#include <iostream>
#include <thread>
#include <atomic>
#include <csignal>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

static std::atomic<bool> shuttingDown{false};
static int serverSockGlobal = -1;

static void handleSignal(int signal) {
    if (signal == SIGINT) {
        shuttingDown.store(true);
        if (serverSockGlobal != -1)
            close(serverSockGlobal);
        std::cout << "\nGraceful shutdown initiated...\n";
    }
}

TcpServer::TcpServer(int port) : port(port) {}

void TcpServer::start() {
    signal(SIGINT, handleSignal);
    Logger::reset();

    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        perror("socket");
        return;
    }

    serverSockGlobal = serverSock;

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind");
        close(serverSock);
        return;
    }

    listen(serverSock, 20);
    std::cout << "Server started. Listening on port " << port << "\n";

    while (!shuttingDown.load()) {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);

        int clientSock = accept(serverSock,
                                (sockaddr*)&clientAddr,
                                &len);

        if (clientSock < 0) {
            if (shuttingDown.load()) break;
            continue;
        }

        std::thread(
            [clientSock, clientAddr]() {
                ClientSession session(clientSock, clientAddr);
                session.run(shuttingDown);
            }
        ).detach();
    }

    close(serverSock);
    std::cout << "Server shutdown complete.\n";
}
