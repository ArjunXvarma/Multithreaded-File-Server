//
// Created by Arjun Varma on 06/01/26.
//

#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP



#include "ThreadPool.hpp"

class TcpServer {
public:
    explicit TcpServer(int port);
    void start();

private:
    int port;
    ThreadPool pool{8, 64};
};



#endif //TCPSERVER_HPP
