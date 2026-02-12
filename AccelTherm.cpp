#include "AccelTherm.h"
#include "GPUwrapper.h"

AccelTherm::AccelTherm(std::string config_file, std::string output_file):
    _configs(config_file) {
    _configFile = config_file;
    _outputFile = output_file;
    _thermStats = new acceltherm::ThermStats(_configs, _outputFile);
    gpuWrapper = new acceltherm::GPUwrapper(_configs, *_thermStats);
}

AccelTherm::~AccelTherm() {
    delete gpuWrapper;
    delete _thermStats;
}

void AccelTherm::finish() {
    //TODO: print some statistics here.
    gpuWrapper->finish();
}

void AccelTherm::calculate(std::string component, int id, int value) {
    gpuWrapper->calculate(component, id, value);
}

void AccelTherm::printAll() {
    _thermStats->printAllStats();
}