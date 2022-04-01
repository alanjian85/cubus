#ifndef CEPHALON_WORLD_GENERATOR_H_
#define CEPHALON_WORLD_GENERATOR_H_

#include <glm/glm.hpp>

#include "blocks/block.h"
#include "chunk.h"

namespace cephalon {
    class World;

    class Generator {
    public:
        void setSeed(unsigned seed);

        void operator()(Chunk& chunk) const;
    };
}

#endif // CEPHALON_WORLD_GENERATOR_H_