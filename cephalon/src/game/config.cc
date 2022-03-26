#include "config.h"
using namespace cephalon;

#include <fstream>

#include <nlohmann/json.hpp>

int Config::viewDistance = 5;
int Config::destroyDistance = 5;
int Config::chunkLoadLimit = 4;
int Config::chunkRebuildLimit = 8;

void Config::init() {
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
    json["viewDistance"] = viewDistance;
    json["destroyDistance"] = destroyDistance;
    json["chunkLoadLimit"] = chunkLoadLimit;
    json["chunkRebuildLimit"] = chunkRebuildLimit;
    return json;
}

void Config::deserialize(const nlohmann::json& json) {
    viewDistance = json["viewDistance"];
    destroyDistance = json["destroyDistance"];
    chunkLoadLimit = json["chunkLoadLimit"];
    chunkRebuildLimit = json["chunkRebuildLimit"];
}