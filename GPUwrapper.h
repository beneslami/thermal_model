#pragma once

#include "config.h"
#include "componentFactory.h"
#include "thermStats.h"

namespace acceltherm {
    class GPUwrapper {
    public:
        GPUwrapper(const Config& configs, ThermStats &stats);
        ~GPUwrapper();
        void calculate(std::string component, int id, int value);
        void finish();
        std::map<std::string, BaseComponent*>_components;
    };
}