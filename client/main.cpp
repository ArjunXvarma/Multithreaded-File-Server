#include "ClientSession.hpp"

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

constexpr const char* SERVER_IP = "127.0.0.1";
constexpr int SERVER_PORT = 9100;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  ./client list\n";
        std::cout << "  ./client put <filename>\n";
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(sock,
                reinterpret_cast<sockaddr*>(&serverAddr),
                sizeof(serverAddr)) < 0) {
        perror("connect");
        return 1;
                }

    std::cout << "Connected to server\n";

    std::string command;

    if (argc == 2) {
        command = argv[1];
    } else if (argc == 3) {
        command = std::string(argv[1]) + " " + argv[2];
    } else {
        std::cerr << "Invalid arguments\n";
        return 1;
    }

    ClientSession session(sock, command);
    session.run();

    return 0;
}
