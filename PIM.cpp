#include "PIM.h"
#include <iostream>

acceltherm::PIM::PIM(const std::map<std::string, std::string> config, acceltherm::ThermStats& stat) : BaseComponent(stat) {
    std::string pim_type = config.at("type");
    std::cout << "Initializing PIM component with configuration " << pim_type << std::endl;
}

acceltherm::PIM::~PIM() {
    // Destructor implementation (if needed)
}

void acceltherm::PIM::calculate(int id, int value) {
    _stats->updateComponentStats("PIM", id, value * 10);
}

void acceltherm::PIM::finish() {
    // Implement the finish logic for PIM
    std::cout << "Finishing PIM component..." << std::endl;
}