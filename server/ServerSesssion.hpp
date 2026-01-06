//
// Created by Arjun Varma on 06/01/26.
//

#ifndef SERVERSESSSION_HPP
#define SERVERSESSSION_HPP



#include <string>
#include <atomic>
#include <netinet/in.h>

class ServerSession {
public:
    ServerSession(int clientSock, sockaddr_in clientAddr);
    void run(std::atomic<bool>& shuttingDown);

private:
    int clientSock;
    std::string clientIP;

    void handleList();
    void handlePut();
};



#endif //SERVERSESSSION_HPP
