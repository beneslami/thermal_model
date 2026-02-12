#pragma once

namespace acceltherm {
    class ThermStats;
    class BaseComponent {
    public:
        BaseComponent(ThermStats& stats) : _stats(&stats) {}
        virtual~BaseComponent() = default;
        virtual void calculate(int, int) = 0;
        virtual void finish() = 0;
    protected:
        acceltherm::ThermStats* _stats;
    };
}