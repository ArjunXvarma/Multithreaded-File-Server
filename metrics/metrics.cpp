//
// Created by Arjun Varma on 06/01/26.
//

#include "metrics.hpp"
#include <sstream>

std::atomic<int> Metrics::activeConnections{0};
std::atomic<long> Metrics::totalRequests{0};
std::atomic<long> Metrics::failedRequests{0};

std::atomic<long long> Metrics::totalLatencyUs{0};
std::atomic<long long> Metrics::completedRequests{0};

void Metrics::connectionOpened() {
    activeConnections.fetch_add(1, std::memory_order_relaxed);
}

void Metrics::connectionClosed() {
    activeConnections.fetch_sub(1, std::memory_order_relaxed);
}

void Metrics::requestStarted() {
    totalRequests.fetch_add(1, std::memory_order_relaxed);
}

void Metrics::requestSucceeded(std::chrono::microseconds latency) {
    completedRequests.fetch_add(1, std::memory_order_relaxed);
    totalLatencyUs.fetch_add(latency.count(), std::memory_order_relaxed);
}

void Metrics::requestFailed() {
    failedRequests.fetch_add(1, std::memory_order_relaxed);
}

std::string Metrics::snapshot() {
    long completed = completedRequests.load();
    long long latency = totalLatencyUs.load();

    long avgLatency =
        completed > 0 ? latency / completed : 0;

    std::ostringstream oss;
    oss << "active_connections=" << activeConnections.load()
        << " total_requests=" << totalRequests.load()
        << " failed_requests=" << failedRequests.load()
        << " avg_latency_us=" << avgLatency;

    return oss.str();
}