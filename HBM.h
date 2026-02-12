#pragma once

#include "thermStats.h"
#include "component.h"
#include <map>
#include <string>

namespace acceltherm
{   
    class ThermStats;
    class HBM : public BaseComponent {
    public:
        HBM(std::map<std::string, std::string> config, ThermStats &stat);
        ~HBM() override;
        void calculate(int id, int value) override;
        void finish() override;
    private:
        int _memCount;
        int _channelCountPerStack;
    };
}