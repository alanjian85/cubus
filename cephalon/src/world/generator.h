#ifndef CEPHALON_WORLD_GENERATOR_H_
#define CEPHALON_WORLD_GENERATOR_H_

#include <glm/glm.hpp>

#include "blocks/block.h"

namespace cephalon {
    class World;

    class Generator {
    public:
        void operator()(World& world, glm::ivec2 region) const;
    };
}

#endif // CEPHALON_WORLD_GENERATOR_H_