#include "GPU.h"
#include <iostream>

acceltherm::GPU::GPU(const std::map<std::string, std::string> config, acceltherm::ThermStats &stat) : BaseComponent(stat) {
    _chipletCount = stoi(config.at("count"));
    std::cout << "GPU initialized with configuration." << std::endl;
}

acceltherm::GPU::~GPU() {
    // Clean up GPU resources
    std::cout << "GPU resources cleaned up." << std::endl;
}

void acceltherm::GPU::calculate(int id, int value) {
    // Perform GPU calculations
    _stats->updateComponentStats("GPU", id, value * 10);
}

void acceltherm::GPU::finish() {
    // Finalize GPU operations
    std::cout << "GPU finishing operations." << std::endl;
}

