#ifndef CEPHALON_WORLD_H_
#define CEPHALON_WORLD_H_

#include <unordered_map>
#include <vector>

#include "aabb.h"
#include "chunk.h"
#include "config.h"
#include "utils.h"

namespace cephalon {
    class World {
    public:
        static Vec3i getChunkOffset(Vec3i pos) {
            return { pos.x / Chunk::kChunkSize.x, 0, pos.z / Chunk::kChunkSize.z };
        }

        static Vec3i getChunkPos(Vec3i pos) {
            return { pos.x % Chunk::kChunkSize.x, pos.y % Chunk::kChunkSize.y, pos.z % Chunk::kChunkSize.z };
        }

        void setGenerator(std::function<void(Vec3i, Chunk&)> generator) {
            generator_ = generator;
        }

        Chunk& getChunk(Vec3i pos) {
            return chunks_[getChunkOffset(pos)];
        }

        void setBlock(Vec3i pos, const Block& block) {
            getChunk(pos).setBlock(getChunkPos(pos), block);
        }

        const Block& getBlock(Vec3i pos) {
            return getChunk(pos).getBlock(getChunkPos(pos));
        }

        void update(bx::Vec3 playerPos);

        void render(bgfx::ProgramHandle program);

        std::vector<AABB> getBoundingBoxes(AABB range);
    private:
        std::unordered_map<Vec3i, Chunk> chunks_;
        std::function<void(Vec3i, Chunk&)> generator_;
    };
}

#endif // CEPHALON_WORLD_H_