#ifndef CEPHALON_GAME_CONFIG_H_
#define CEPHALON_GAME_CONFIG_H_

#include <nlohmann/json.hpp>

namespace cephalon {
    class Config {
    public:
        static void init();
        
        static void cleanup();

        static int viewDistance;
        static int destroyDistance;
        static int chunkLoadLimit;
        static int chunkRebuildLimit;
    private:
        static nlohmann::json serialize();
        static void deserialize(const nlohmann::json& json);
    };
}

#endif // CEPHALON_GAME_CONFIG_H_