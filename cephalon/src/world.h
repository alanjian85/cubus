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
    private:
        std::unordered_map<Vec3i, Chunk> chunks_;
    };
}

#endif // CEPHALON_WORLD_H_