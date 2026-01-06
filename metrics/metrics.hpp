//
// Created by Arjun Varma on 06/01/26.
//

#ifndef METRICS_HPP
#define METRICS_HPP



#include <atomic>
#include <cstdint>
#include <string>

class Metrics {
public:
    // Lifecycle
    static void clientConnected();
    static void clientDisconnected();

    // Requests
    static void requestReceived();
    static void requestCompleted();
    static void requestFailed();

    // File ops
    static void fileUploadStarted();
    static void fileUploadCompleted();
    static void fileUploadFailed();

    // Snapshot
    static std::string snapshot();

private:
    static std::atomic<uint64_t> activeConnections;
    static std::atomic<uint64_t> totalRequests;
    static std::atomic<uint64_t> completedRequests;
    static std::atomic<uint64_t> failedRequests;
    static std::atomic<uint64_t> uploadsStarted;
    static std::atomic<uint64_t> uploadsCompleted;
    static std::atomic<uint64_t> uploadsFailed;
};




#endif //METRICS_HPP
