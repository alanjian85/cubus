#ifndef CEPHALON_BLOCK_H_
#define CEPHALON_BLOCK_H_

#include <glm/glm.hpp>

#include "aabb.h"
#include "utils.h"

namespace cephalon {
    class Block {
    public:
        virtual ~Block() noexcept = default;

        virtual bool isAir() const {
            return false;
        }

        virtual glm::vec2 getTexCoord() const = 0;

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
        extern const Air kAir;
        extern const Dirt kDirt;
        extern const Grass kGrass;
        extern const Stone kStone;
        extern const Wood kWood;
    }
}

#endif // CEPHALON_BLOCK_H_