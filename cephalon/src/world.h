#ifndef CEPHALON_WORLD_H_
#define CEPHALON_WORLD_H_

#include <unordered_map>

#include "chunk.h"
#include "utils.h"

namespace cephalon {
    class World {
    public:
        static Vec3i getChunkPos(Vec3i pos) {
            return { pos.x % Chunk::kChunkSize.x, pos.y % Chunk::kChunkSize.y, pos.z % Chunk::kChunkSize.z };
        }

        Chunk& getChunk(Vec3i pos) {
            return chunks_[{pos.x / Chunk::kChunkSize.x, pos.y / Chunk::kChunkSize.y, pos.z / Chunk::kChunkSize.z}];
        }

        void setBlock(Vec3i pos, const Block& block) {
            getChunk(pos).setBlock(getChunkPos(pos), block);
        }

        const Block& getBlock(Vec3i pos) {
            return getChunk(pos).getBlock(getChunkPos(pos));
        }

        void update() {
            for (auto& [_, chunk] : chunks_) {
                chunk.update();
            }
        }

        void render(bgfx::ProgramHandle program);

        void clear() {
            chunks_.clear();
        }
    private:
        std::unordered_map<Vec3i, Chunk> chunks_;
    };
}

#endif // CEPHALON_WORLD_H_