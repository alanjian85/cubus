#pragma once

#include <map>
#include <string>

#include <nlohmann/json.hpp>

namespace cephalon {
    class Config {
    public:
        static int viewDistance;
        static int destroyDistance;
        static int chunkLoadLimit;

        static void init();
        
        static void deinit();
    private:
        static std::map<void*, std::string> keys_;

        static nlohmann::json serialize();
        static void deserialize(const nlohmann::json& json);
    };
}
