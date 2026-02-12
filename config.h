#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <map>
#include <cctype>

namespace acceltherm
{
    class Config {
    private:
        std::map<std::string, std::map<std::string, std::string>> _options;
        static std::string trim(std::string s) {
            auto is_ws = [](unsigned char c) { return std::isspace(c) != 0; };
            size_t b = 0;
            while (b < s.size() && is_ws((unsigned char)s[b])) b++;
            size_t e = s.size();
            while (e > b && is_ws((unsigned char)s[e - 1])) e--;
            return s.substr(b, e - b);
        }
        static bool split_kv(const std::string& line, std::string& key, std::string& val) {
            const auto pos = line.find('=');
            if (pos == std::string::npos) return false;
            key = trim(line.substr(0, pos));
            val = trim(line.substr(pos + 1));
            return !key.empty();
        }
    public:
        Config() = default;
        explicit Config(const std::string& fname);
        void parse(const std::string& fname);
        std::vector<std::string> getComponents() const {
            std::vector<std::string> components;
            for (const auto& kv : _options) {
                components.push_back(kv.first);
            }
            return components;
        }
        
        std::map<std::string, std::string> getComponent(const std::string &component) const {
            auto it = _options.find(component);
            return (it == _options.end()) ? std::map<std::string, std::string>() : it->second;
        }
    };
}