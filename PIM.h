#pragma once

#include "thermStats.h"
#include "component.h"
#include <map>
#include <string>

namespace acceltherm
{   
    class ThermStats;
    class PIM : public BaseComponent {
    public:
        PIM(std::map<std::string, std::string> config, ThermStats &stat);
        ~PIM() override;
        void calculate(int id, int value) override;
        void finish() override;
    };
}