//
// Created by Arjun Varma on 06/01/26.
//

#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP



class TcpServer {
public:
    explicit TcpServer(int port);
    void start();

private:
    int port;
};



#endif //TCPSERVER_HPP
