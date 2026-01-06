//
// Created by Arjun Varma on 06/01/26.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

class Logger {
public:
    static void reset();
    static void log(const std::string& clientIP, const std::string& request);
};


#endif //LOGGER_HPP
