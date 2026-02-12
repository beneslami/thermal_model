#include "config.h"

acceltherm::Config::Config(const std::string& fname) {
  parse(fname);
}

void acceltherm::Config::parse(const std::string& fname) {
    std::ifstream infile(fname);
    if (!infile.is_open()) {
        std::cerr << "Error opening config file: " << fname << std::endl;
        return;
    }
    _options.clear();
    std::string current_component; // active section
    std::string line;
    size_t lineno = 0;

    while (std::getline(infile, line)) {
        lineno++;
        // Strip comments (# or //)
        {
            auto hash = line.find('#');
            auto sl   = line.find("//");
            size_t cut = std::string::npos;
            if (hash != std::string::npos) cut = hash;
            if (sl   != std::string::npos) cut = std::min(cut, sl);
            if (cut != std::string::npos) line = line.substr(0, cut);
        }
        line = trim(line);
        if (line.empty()) continue;
        std::string key, val;
        if (!split_kv(line, key, val)) {
            throw std::runtime_error("Config::parse: bad line (missing '=') at " + std::to_string(lineno) + ": " + line);
            exit(EXIT_FAILURE);
        }
        if (key == "component") {
            if (val.empty()) {
                throw std::runtime_error("Config::parse: empty component at line " + std::to_string(lineno));
            }
            current_component = val;
            // Ensure the component exists in the map even if it has no options yet
            _options[current_component];
            continue;
        }
        if (current_component.empty()) {
            throw std::runtime_error("Config::parse: key before any component at line " + std::to_string(lineno) + ": " + key);
        }
        _options[current_component][key] = val;
    }
    infile.close();
}