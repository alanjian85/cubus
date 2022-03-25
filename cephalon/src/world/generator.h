#ifndef CEPHALON_WORLD_GENERATOR_H_
#define CEPHALON_WORLD_GENERATOR_H_

#include <glm/glm.hpp>

#include "chunk.h"
#include "blocks/block.h"

namespace cephalon {
    class Generator {
    public:
        const Block& operator()(glm::ivec3 pos) const {
            if (pos.y == 0)
                return blocks::stone;
            if (pos.y >= 1 && pos.y <= 4)
                return blocks::dirt;
            if (pos.y == 5)
                return blocks::grass;
            return blocks::air;
        }
    };
}

#endif // CEPHALON_WORLD_GENERATOR_H_