//
// Created by Arjun Varma on 06/01/26.
//

#include "Logger.hpp"

#include <fstream>
#include <mutex>
#include <ctime>

namespace {
    std::mutex logMutex;
}

void Logger::reset() {
    std::lock_guard<std::mutex> lock(logMutex);
    std::ofstream("log.txt", std::ios::trunc);
}

void Logger::log(const std::string& clientIP, const std::string& request) {
    std::lock_guard<std::mutex> lock(logMutex);

    std::ofstream logFile("log.txt", std::ios::app);
    if (!logFile.is_open()) {
        return; // fail silently; logging must never crash the server
    }

    std::time_t now = std::time(nullptr);
    logFile << std::ctime(&now)
            << " | " << clientIP
            << " | " << request
            << "\n";
}
