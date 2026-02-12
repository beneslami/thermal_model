#include "HBM.h"
#include <iostream>

acceltherm::HBM::HBM(const std::map<std::string, std::string> config, acceltherm::ThermStats &stat) : BaseComponent(stat) {
    _memCount = stoi(config.at("count"));
    _channelCountPerStack = stoi(config.at("channel"));
    std::cout << "Initializing HBM component with configuration:" << std::endl;
}

acceltherm::HBM::~HBM() {
    // Destructor implementation (if needed)
}

void acceltherm::HBM::calculate(int id, int value) {
    _stats->updateComponentStats("HBM", id, value * 10);
}

void acceltherm::HBM::finish() {
    std::cout << "Finishing HBM component..." << std::endl;
}