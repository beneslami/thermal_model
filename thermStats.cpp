#include "thermStats.h"
#include <iostream>

acceltherm::ThermStats::ThermStats(acceltherm::Config &config, const std::string outputFile) : _config(config), _outputFile(outputFile) {
    std::vector<std::string>components = _config.getComponents();
    for (const auto& component : components) {
        std::map<std::string, std::string>component_info = _config.getComponent(component);
        int count = stoi(component_info.at("count"));
        for(int i = 0; i < count; ++i) {
            _componentStats[component][i].push_back(0.0f); // Initialize stats with a default value (e.g., 0.0)
        }
    }
    if(_outputFile.empty()) {
        _out = &std::cout;
    } else {
        _file = std::make_unique<std::ofstream>(_outputFile, std::ios::out | std::ios::trunc);
        if (!*_file) {
            throw std::runtime_error("ThermStats: failed to open output file: " + _outputFile);
            exit(EXIT_FAILURE);
        }
        _out = _file.get();
    }
}

void acceltherm::ThermStats::updateComponentStats(const std::string& componentName, int id, float statValue) {
    auto it = _componentStats.find(componentName);
    if (it != _componentStats.end()) {
        auto it2 = it->second.find(id);
        if(it2 != it->second.end()){
            it2->second.push_back(statValue);
        }
        if(_outputFile.empty()){
            std::cout << "component " << componentName << " ID " << id << " - temperature: " << statValue << "C\n";
        }
    }
}

void acceltherm::ThermStats::printAllStats() {
    for (const auto& entry : _componentStats) {
        (*_out) << "Component: " << entry.first << "\n";
        for (const auto &id_num : entry.second) {
            (*_out) << "ID " << id_num.first << ": ";
            for(const auto &stat : id_num.second) {
                (*_out) << stat << " ";
            }
            (*_out) << "\n";
        }
        (*_out) << "\n\n";
    }
}

void acceltherm::ThermStats::printComponentStats(const std::string& componentName) {
    auto it = _componentStats.find(componentName);
    if (it != _componentStats.end()) {
        (*_out) << "Component: " << it->first << "\n";
        for (const auto& id_num : it->second) {
            (*_out) << "ID: " << id_num.first;
            for (const auto& stat : id_num.second) {
                (*_out) << stat << " ";
            }
            (*_out) << "\n";
        }
        (*_out) << "\n\n";
    } else {
        (*_out) << "Component '" << componentName << "' not found.\n";
        exit(EXIT_FAILURE);
    }
}


