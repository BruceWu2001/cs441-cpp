#include "../../headers/utils/printMessage.h"

std::mutex coutMutex;  // Global mutex
void printMessage(const std::string& message) {
    // Lock the mutex before printing
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << message << std::endl;
}