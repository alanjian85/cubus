#ifndef CEPHALON_WORLD_H_
#define CEPHALON_WORLD_H_

#include <cmath>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "aabb.h"
#include "chunk.h"
#include "config.h"
#include "generator.h"

namespace cephalon {
    class World {
    public:
        static glm::ivec3 getRegion(glm::ivec3 pos) {
            return glm::floor(glm::vec3(pos) / glm::vec3(Chunk::kVolume));
        }

        static glm::ivec3 getChunkPos(glm::ivec3 pos) {
            return (pos % Chunk::kVolume + Chunk::kVolume) % Chunk::kVolume;
        }

        void setBlock(glm::ivec3 pos, const Block& block) {
            blocks_[pos] = &block;
            auto it = chunks_.find(getRegion(pos));
            if (it != chunks_.cend())
                it->second.setBlock(getChunkPos(pos), block);
        }

        const Block& getBlock(glm::ivec3 pos) const {
            auto it = blocks_.find(pos);
            if (it != blocks_.cend())
                return *it->second;
            else
                return generator_(pos);
        }

        void update(glm::vec3 playerPos);

        void render();

        std::vector<std::pair<glm::ivec3, AABB>> getBoundingBoxes(AABB range);
    private:
        void loadChunk(glm::ivec3 region, Chunk& chunk);

        std::unordered_map<glm::ivec3, Chunk> chunks_;
        std::unordered_map<glm::ivec3, const Block*> blocks_;
        Generator generator_;
    };
}

#endif // CEPHALON_WORLD_H_