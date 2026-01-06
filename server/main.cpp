//
// Created by Arjun Varma on 06/01/26.
//

#include "TCPServer.hpp"

constexpr int PORT = 9100;

int main() {
    TcpServer server(PORT);
    server.start();
    return 0;
}
