//
// Created by Arjun Varma on 06/01/26.
//

#ifndef CLIENTSESSION_HPP
#define CLIENTSESSION_HPP



#include <string>

class ClientSession {
public:
    explicit ClientSession(int sock, std::string command);
    void run();

private:
    int sockfd;
    std::string command;

    void handleList();
    void handlePut(const std::string& filename);
};



#endif //CLIENTSESSION_HPP
