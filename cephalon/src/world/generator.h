#ifndef CEPHALON_WORLD_GENERATOR_H_
#define CEPHALON_WORLD_GENERATOR_H_

#include <glm/glm.hpp>

#include "chunk.h"
#include "blocks/block.h"

namespace cephalon {
    class Generator {
    public:
        void operator()(Chunk& chunk) const {
            for (int x = 0; x < Chunk::kVolume.x; ++x) {
                for (int y = 0; y < Chunk::kVolume.y; ++y) {
                    for (int z = 0; z < Chunk::kVolume.z; ++z) {
                        glm::ivec3 offset(x, y, z);
                        if (y == 0)
                            chunk.setBlock(offset, blocks::stone);
                        else if (y > 0 && y < 5)
                            chunk.setBlock(offset, blocks::dirt);
                        else if (y == 5)
                            chunk.setBlock(offset, blocks::grass);
                    }
                }
            }
        }
    };
}

#endif // CEPHALON_WORLD_GENERATOR_H_