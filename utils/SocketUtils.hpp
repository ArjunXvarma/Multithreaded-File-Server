//
// Created by Arjun Varma on 06/01/26.
//

#ifndef SOCKETUTILS_HPP
#define SOCKETUTILS_HPP



#include <string>

class SocketUtils {
public:
    static std::string recvLine(int sockfd);
    static void setReceiveTimeout(int sockfd, int seconds);
};


#endif //SOCKETUTILS_HPP
