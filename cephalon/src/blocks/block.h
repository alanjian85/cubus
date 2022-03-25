#ifndef CEPHALON_BLOCKS_BLOCK_H_
#define CEPHALON_BLOCKS_BLOCK_H_

#include <glm/glm.hpp>

#include "utils/aabb.h"
#include "utils/assets.h"
#include "utils/atlas.h"

namespace cephalon {
    class Block {
    public:
        static void init(Atlas& atlas);

        virtual ~Block() noexcept = default;

        virtual bool isAir() const {
            return false;
        }

        virtual Region getRegion() const = 0;

        virtual AABB getBoundingBox(glm::ivec3 pos) const {
            return AABB(glm::vec3(pos) - 0.5f, glm::vec3(pos) + 0.5f);
        }
    };
}

#include "air.h"
#include "dirt.h"
#include "grass.h"
#include "stone.h"
#include "wood.h"

namespace cephalon {
    namespace blocks {
        extern Air air;
        extern Dirt dirt;
        extern Grass grass;
        extern Stone stone;
        extern Wood wood;
    }
}

#endif // CEPHALON_BLOCKS_BLOCK_H_