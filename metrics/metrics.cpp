//
// Created by Arjun Varma on 06/01/26.
//

#include "metrics.hpp"

#include "metrics.hpp"
#include <sstream>

std::atomic<uint64_t> Metrics::activeConnections{0};
std::atomic<uint64_t> Metrics::totalRequests{0};
std::atomic<uint64_t> Metrics::completedRequests{0};
std::atomic<uint64_t> Metrics::failedRequests{0};
std::atomic<uint64_t> Metrics::uploadsStarted{0};
std::atomic<uint64_t> Metrics::uploadsCompleted{0};
std::atomic<uint64_t> Metrics::uploadsFailed{0};

void Metrics::clientConnected() {
    activeConnections++;
}

void Metrics::clientDisconnected() {
    activeConnections--;
}

void Metrics::requestReceived() {
    totalRequests++;
}

void Metrics::requestCompleted() {
    completedRequests++;
}

void Metrics::requestFailed() {
    failedRequests++;
}

void Metrics::fileUploadStarted() {
    uploadsStarted++;
}

void Metrics::fileUploadCompleted() {
    uploadsCompleted++;
}

void Metrics::fileUploadFailed() {
    uploadsFailed++;
}

std::string Metrics::snapshot() {
    std::ostringstream oss;
    oss << "ActiveConnections=" << activeConnections.load()
        << " | TotalRequests=" << totalRequests.load()
        << " | CompletedRequests=" << completedRequests.load()
        << " | FailedRequests=" << failedRequests.load()
        << " | UploadsStarted=" << uploadsStarted.load()
        << " | UploadsCompleted=" << uploadsCompleted.load()
        << " | UploadsFailed=" << uploadsFailed.load();
    return oss.str();
}