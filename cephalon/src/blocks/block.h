#ifndef CEPHALON_BLOCK_H_
#define CEPHALON_BLOCK_H_

#include <cstdint>

#include "aabb.h"
#include "utils.h"

namespace cephalon {
    class Block {
    public:
        virtual ~Block() noexcept = default;

        virtual bool isAir() const {
            return false;
        }

        virtual std::uint32_t getColor() const = 0;

        virtual AABB getBoundingBox(Vec3i pos) const {
            return AABB(bx::sub(pos, bx::Vec3(0.5f)), bx::add(pos, bx::Vec3(0.5f)));
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