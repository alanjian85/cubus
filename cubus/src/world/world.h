#pragma once

#include <cmath>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <set>
#include <shared_mutex>
#include <vector>

#include <boost/asio.hpp>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "utils/aabb.h"
#include "chunk.h"
#include "database.h"
#include "game/config.h"

namespace cephalon {
    class Terrain;

    class World {
    public:
        static glm::ivec2 getRegion(glm::ivec3 pos) {
            return glm::ivec2(
                glm::floor(static_cast<float>(pos.x) / Chunk::kVolume.x),
                glm::floor(static_cast<float>(pos.z) / Chunk::kVolume.z)
            );
        }

        static glm::ivec3 getOffset(glm::ivec3 pos) {
            return (pos % Chunk::kVolume + Chunk::kVolume) % Chunk::kVolume;
        }

        static glm::ivec3 getPosition(glm::ivec2 region, glm::ivec3 offset) {
            return glm::ivec3(region.x, 0, region.y) * Chunk::kVolume + offset;
        }

        World(const char* save_path);

        ~World();

        void setTerrain(const std::string& name);

        void setSeed(unsigned seed);

        void setBlock(glm::ivec3 pos, const Block& block);

        const Block* getBlock(glm::ivec3 pos) const;

        void update(glm::vec3 player_pos);

        void render(PerspectiveCamera cam);

        bool intersect(PerspectiveCamera cam, Direction& dir, glm::ivec3& pos) const;
    private:
        void setChunkDirty(glm::ivec2 region, bool dirty);

        mutable std::shared_mutex mutex_;
        std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>> chunks_;

        boost::asio::thread_pool rebuild_thread_pool_;

        Terrain* terrain_;

        Database database_;
    };
}
