#ifndef CEPHALON_WORLD_TERRAIN_H_
#define CEPHALON_WORLD_TERRAIN_H_

#include <glm/glm.hpp>

#include "blocks/block.h"
#include "world/chunk.h"

namespace cephalon {
    class World;

    class Terrain {
    public:
        void setSeed(unsigned seed);

        void genChunk(Chunk& chunk) const;
    };
}

#endif // CEPHALON_WORLD_TERRAIN_H_