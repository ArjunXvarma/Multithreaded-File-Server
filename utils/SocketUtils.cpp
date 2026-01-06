//
// Created by Arjun Varma on 06/01/26.
//

#include "SocketUtils.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

std::string SocketUtils::recvLine(int sockfd) {
    std::string line;
    char c;

    while (true) {
        int n = recv(sockfd, &c, 1, 0);

        if (n == 0) {
            // Peer closed connection
            return "";
        }

        if (n < 0) {
            // Timeout or socket error
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                return "";
            }
            return "";
        }

        if (c == '\n') {
            break;
        }

        line += c;
    }

    return line;
}

void SocketUtils::setReceiveTimeout(int sockfd, int seconds) {
    timeval timeout{};
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;

    setsockopt(sockfd,
               SOL_SOCKET,
               SO_RCVTIMEO,
               &timeout,
               sizeof(timeout));
}
