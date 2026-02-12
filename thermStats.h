#pragma once

#include <cstring>
#include "componentFactory.h"
#include "config.h"
#include <fstream>
#include <ostream>
#include <iostream>
#include <vector>
#include <list>
#include <map>

namespace acceltherm {
    class ThermStats {
    public:
        ThermStats(acceltherm::Config &config, const std::string outputFile);
        ~ThermStats() = default;
        void printAllStats();
        void printComponentStats(const std::string& componentName);
        void updateComponentStats(const std::string& componentName, int id, float statValue);
    private:
        Config &_config;
        std::unique_ptr<std::ofstream> _file;
        std::ostream* _out = &std::cout;
        const std::string _outputFile;
        std::map<std::string, std::map<int, std::list<float>>> _componentStats; // Map to hold component names and their corresponding stats
    };
}