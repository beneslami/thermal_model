#pragma once

#include "thermStats.h"
#include "component.h"
#include <map>
#include <string>

namespace acceltherm
{
    class ThermStats;
    class GPU : public BaseComponent {
    public:
        GPU(std::map<std::string, std::string> config, ThermStats &stat);
        ~GPU() override;
        void calculate(int id, int value) override;
        void finish() override;
    private:
        int _chipletCount;
    };
}