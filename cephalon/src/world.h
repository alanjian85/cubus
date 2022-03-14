#ifndef CEPHALON_WORLD_H_
#define CEPHALON_WORLD_H_

#include <cmath>
#include <unordered_map>
#include <vector>

#include "aabb.h"
#include "chunk.h"
#include "config.h"
#include "generator.h"
#include "utils.h"

namespace cephalon {
    class World {
    public:
        static Vec3i getChunkOffset(Vec3i pos) {
            return Vec3i(
                std::floor(static_cast<float>(pos.x) / Chunk::kChunkSize.x),
                0,
                std::floor(static_cast<float>(pos.z) / Chunk::kChunkSize.z)
            );
        }

        static Vec3i getChunkPos(Vec3i pos) {
            return (pos % Chunk::kChunkSize + Chunk::kChunkSize) % Chunk::kChunkSize;
        }

        void setBlock(Vec3i pos, const Block& block) {
            blocks_[pos] = &block;
            auto it = chunks_.find(getChunkOffset(pos));
            if (it != chunks_.cend())
                it->second.setBlock(getChunkPos(pos), block);
        }

        const Block& getBlock(Vec3i pos) const {
            auto it = blocks_.find(pos);
            if (it != blocks_.cend())
                return *it->second;
            else
                return generator_(pos);
        }

        void update(bx::Vec3 playerPos);

        void render(bgfx::ProgramHandle program);

        std::vector<std::pair<Vec3i, AABB>> getBoundingBoxes(AABB range);
    private:
        void loadChunk(Vec3i offset, Chunk& chunk);

        std::unordered_map<Vec3i, Chunk> chunks_;
        std::unordered_map<Vec3i, const Block*> blocks_;
        Generator generator_;
    };
}

#endif // CEPHALON_WORLD_H_