#include "componentFactory.h"
#include <cstring>

namespace acceltherm {
    template<>
    BaseComponent* ComponentFactory<HBM>::createComponent(std::map<std::string, std::string> componentConfig, ThermStats &stats) {
        acceltherm::HBM *hbm = new acceltherm::HBM(componentConfig, stats);
        validateComponent(componentConfig);
        return hbm;
    }
    
    template<>
    BaseComponent* ComponentFactory<GPU>::createComponent(std::map<std::string, std::string> componentConfig, ThermStats &stats) {
        acceltherm::GPU *gpu = new acceltherm::GPU(componentConfig, stats);
        validateComponent(componentConfig);
        return gpu;
    }

    template<>
    BaseComponent* ComponentFactory<PIM>::createComponent(std::map<std::string, std::string> componentConfig, ThermStats &stats) {
        acceltherm::PIM *pim = new acceltherm::PIM(componentConfig, stats);
        validateComponent(componentConfig);
        return pim;
    }

    template<>
    void ComponentFactory<HBM>::validateComponent(std::map<std::string, std::string> componentConfig) {
        int channel = std::stoi(componentConfig.at("channel"));
        int count = std::stoi(componentConfig.at("count"));
        assert(channel == 8 && channel != 0);
        assert(count > 0 && count % 2 == 0);
    }

    template<>
    void ComponentFactory<GPU>::validateComponent(std::map<std::string, std::string> componentConfig) {
        int chiplet_num = std::stoi(componentConfig.at("count"));
        assert(chiplet_num > 0);
    }

    template<>
    void ComponentFactory<PIM>::validateComponent(std::map<std::string, std::string> componentConfig) {
        int count = std::stoi(componentConfig.at("count"));
        assert(count > 0);
    }
}