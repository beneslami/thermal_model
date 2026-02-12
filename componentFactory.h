#pragma once

#include "thermStats.h"
#include "component.h"
#include "config.h"
#include "GPU.h"
#include "HBM.h"
#include "PIM.h"

namespace acceltherm {

    class ThermStats; // forward decl
    class HBM;
    class GPU;
    class PIM;
    
    template <typename T>
    class ComponentFactory {
    public:
        static BaseComponent* createComponent(const std::map<std::string, std::string> componentConfig, ThermStats &stats);
        static void validateComponent(const std::map<std::string, std::string> componentConfig);
    protected:

    };
    template<>
    BaseComponent* ComponentFactory<HBM>::createComponent(std::map<std::string, std::string> componentConfig, ThermStats &stats);
    template<>
    BaseComponent* ComponentFactory<GPU>::createComponent(std::map<std::string, std::string> componentConfig, ThermStats &stats);
    template<>
    BaseComponent* ComponentFactory<PIM>::createComponent(std::map<std::string, std::string> componentConfig, ThermStats &stats);
    template<>
    void ComponentFactory<HBM>::validateComponent(std::map<std::string, std::string> componentConfig);
    template<>
    void ComponentFactory<GPU>::validateComponent(std::map<std::string, std::string> componentConfig);
    template<>
    void ComponentFactory<PIM>::validateComponent(std::map<std::string, std::string> componentConfig);
}