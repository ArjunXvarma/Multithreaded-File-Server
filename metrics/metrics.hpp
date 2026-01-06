//
// Created by Arjun Varma on 06/01/26.
//

#ifndef METRICS_HPP
#define METRICS_HPP



#include <atomic>
#include <chrono>
#include <string>

class Metrics {
public:
    static void connectionOpened();
    static void connectionClosed();
    static void connectionRejected();

    static void requestStarted();
    static void requestSucceeded(std::chrono::microseconds latency);
    static void requestFailed();

    static std::string snapshot();

private:
    static std::atomic<int> activeConnections;
    static std::atomic<long> totalRequests;
    static std::atomic<long> failedRequests;

    static std::atomic<long long> totalLatencyUs;
    static std::atomic<long long> completedRequests;
};




#endif //METRICS_HPP
