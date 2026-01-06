//
// Created by Arjun Varma on 06/01/26.
//

#ifndef CLIENTSESSION_HPP
#define CLIENTSESSION_HPP



#include <string>
#include <atomic>
#include <netinet/in.h>

class ClientSession {
public:
    ClientSession(int sock, sockaddr_in addr);
    void run(std::atomic<bool>& shuttingDown);

private:
    int clientSock;
    std::string clientIP;
};



#endif //CLIENTSESSION_HPP
