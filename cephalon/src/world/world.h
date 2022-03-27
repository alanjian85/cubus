#ifndef CEPHALON_WORLD_WORLD_H_
#define CEPHALON_WORLD_WORLD_H_

#include <cmath>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <set>
#include <shared_mutex>
#include <vector>

#include <boost/asio.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "utils/aabb.h"
#include "chunk.h"
#include "game/config.h"
#include "generator.h"

namespace cephalon {
    class World {
    public:
        static glm::ivec2 getRegion(glm::ivec3 pos) {
            return glm::ivec2(
                glm::floor(static_cast<float>(pos.x) / Chunk::kVolume.x),
                glm::floor(static_cast<float>(pos.z) / Chunk::kVolume.z)
            );
        }

        static glm::ivec3 getOffset(glm::ivec3 pos) {
            auto region = getRegion(pos);
            return pos - glm::ivec3(region.x, 0, region.y) * Chunk::kVolume;
        }

        static glm::ivec3 getPosition(glm::ivec2 region, glm::ivec3 offset) {
            return glm::ivec3(region.x, 0, region.y) * Chunk::kVolume + offset;
        }

        World();

        ~World();

        void setChunkDirty(glm::ivec2 region, bool dirty) {
            std::lock_guard lock(mutex_);
            auto it = chunks_.find(region);
            if (it != chunks_.cend()) {
                it->second->setDirty(dirty);
            }
        }

        void setBlock(glm::ivec3 pos, const Block& block) {
            std::lock_guard lock(mutex_);
            auto it = chunks_.find(getRegion(pos));
            if (it != chunks_.cend()) {
                it->second->setBlock(getOffset(pos), block);
            }
        }

        const Block* getBlock(glm::ivec3 pos) const {
            std::lock_guard lock(mutex_);
            auto it = chunks_.find(getRegion(pos));
            if (it != chunks_.cend()) {
                return &it->second->getBlock(getOffset(pos));
            }
            return nullptr;
        }

        void update(glm::vec3 player_pos);

        void render(PerspectiveCamera cam);

        bool intersect(PerspectiveCamera cam, Direction& dir, glm::ivec3& pos) const;
    private:
        mutable std::mutex mutex_;
        std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>> chunks_;
        std::unordered_map<glm::ivec3, const Block*> blocks_;

        boost::asio::thread_pool thread_pool_;

        Generator generator_;
    };
}

#endif // CEPHALON_WORLD_WORLD_H_