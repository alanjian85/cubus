#ifndef CEPHALON_WORLD_WORLD_H_
#define CEPHALON_WORLD_WORLD_H_

#include <cmath>
#include <unordered_map>
#include <set>
#include <vector>

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

        Chunk* getChunk(glm::ivec2 region) {
            auto it = chunks_.find(region);
            if (it != chunks_.cend())
                return &it->second;
            return nullptr;
        }

        const Chunk* getChunk(glm::ivec2 region) const {
            auto it = chunks_.find(region);
            if (it != chunks_.cend())
                return &it->second;
            return nullptr;
        }

        void setBlock(glm::ivec3 pos, const Block& block) {
            blocks_[pos] = &block;
            if (auto chunk = getChunk(getRegion(pos)))
                chunk->setBlock(getOffset(pos), block);
        }

        const Block& getBlock(glm::ivec3 pos) const {
            auto it = blocks_.find(pos);
            if (it != blocks_.cend())
                return *it->second;
            else
                return generator_(pos);
        }

        void update(glm::vec3 player_pos);

        void render(PerspectiveCamera cam);

        bool intersect(PerspectiveCamera cam, Direction& dir, glm::ivec3& pos) const;
    private:
        void loadChunk(glm::ivec2 region, Chunk& chunk);

        std::unordered_map<glm::ivec2, Chunk> chunks_;
        std::unordered_map<glm::ivec3, const Block*> blocks_;
        Generator generator_;
    };
}

#endif // CEPHALON_WORLD_WORLD_H_