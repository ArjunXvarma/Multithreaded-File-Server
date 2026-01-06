//
// Created by Arjun Varma on 06/01/26.
//

#ifndef FILESERVICE_HPP
#define FILESERVICE_HPP



#include <string>

class FileService {
public:
    static void listFiles(int clientSock, const std::string& clientIP);
    static void receiveFile(int clientSock, const std::string& clientIP);

private:
    static void sendLine(int sockfd, const std::string& msg);
};



#endif //FILESERVICE_HPP
