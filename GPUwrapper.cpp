#include "GPUwrapper.h"
#include "componentFactory.h"
#include "thermStats.h"
#include <functional>
#include <map>
#include <string>
#include "config.h"
#include "GPU.h"
#include "HBM.h"
#include "PIM.h"

static std::map<std::string, std::function<acceltherm::BaseComponent*(std::map<std::string, std::string>, acceltherm::ThermStats& )>> componentFactoryMap = {
    {"GPU", &acceltherm::ComponentFactory<acceltherm::GPU>::createComponent},
    {"HBM", &acceltherm::ComponentFactory<acceltherm::HBM>::createComponent},
    {"PIM", &acceltherm::ComponentFactory<acceltherm::PIM>::createComponent},
};

acceltherm::GPUwrapper::GPUwrapper(const acceltherm::Config& configs, acceltherm::ThermStats &stats) {
    std::vector<std::string> components = configs.getComponents();
    std::vector<std::string>::iterator it;
    for(it = components.begin(); it != components.end(); ++it) {
        std::string component = *it;
        if (componentFactoryMap.find(component) != componentFactoryMap.end()) {
            std::map<std::string, std::string> componentConfig = configs.getComponent(component);
            _components[component] = componentFactoryMap[component](componentConfig, stats);
        } else {
            std::cerr << "Unknown component: " << component << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

acceltherm::GPUwrapper::~GPUwrapper() {
    std::map<std::string, BaseComponent*>::iterator it;
    for (it = _components.begin(); it != _components.end(); ++it) {
        delete it->second;  
    }
}

void acceltherm::GPUwrapper::calculate(std::string component, int id, int value) {
    if (_components.find(component) != _components.end()) {
        _components[component]->calculate(id, value);
    } else {
        std::cerr << "Unknown component: " << component << std::endl;
        exit(EXIT_FAILURE);
    }
}

void acceltherm::GPUwrapper::finish() {
    std::map<std::string, BaseComponent*>::iterator it;
    for (it = _components.begin(); it != _components.end(); ++it) {
        it->second->finish();
    }
}