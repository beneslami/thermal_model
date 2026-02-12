#pragma once

#include <string>
#include "config.h"
#include "GPUwrapper.h"
#include "thermStats.h"

namespace acceltherm {
    class GPUwrapper;
    class ThermStats;
}

class AccelTherm {
public:
    AccelTherm(std::string config_file, std::string output_file);
    ~AccelTherm();
    void finish();
    void calculate(std::string component, int id, int value);
    void printAll();
    //void calculateAll();
    acceltherm::GPUwrapper *gpuWrapper;
private:
    std::string _configFile;
    acceltherm::Config _configs;
    std::string _outputFile;
    acceltherm::ThermStats* _thermStats;
};