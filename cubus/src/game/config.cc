#include "config.h"
using namespace cephalon;

#include <fstream>

#include <nlohmann/json.hpp>

int Config::viewDistance = 12;
int Config::destroyDistance = 5;
int Config::chunkLoadLimit = 4;

std::map<void*, std::string> Config::keys_;

void Config::init() {
    keys_[&viewDistance] = "viewDistance";
    keys_[&destroyDistance] = "destroyDistance";
    keys_[&chunkLoadLimit] = "chunkLoadLimit";

    std::ifstream file("config.json");
    if (file.is_open()) {
        nlohmann::json json;
        file >> json;
        deserialize(json);
    }
}

void Config::deinit() {
    std::ofstream file("config.json");
    file << serialize().dump(4);
}

nlohmann::json Config::serialize() {
    nlohmann::json json;
    json[keys_[&viewDistance]] = viewDistance;
    json[keys_[&destroyDistance]] = destroyDistance;
    json[keys_[&chunkLoadLimit]] = chunkLoadLimit;
    return json;
}

void Config::deserialize(const nlohmann::json& json) {
    viewDistance = json[keys_[&viewDistance]];
    destroyDistance = json[keys_[&destroyDistance]];
    chunkLoadLimit = json[keys_[&chunkLoadLimit]];
}