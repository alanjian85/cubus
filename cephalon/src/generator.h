#ifndef CEPHALON_GENERATOR_H_
#define CEPHALON_GENERATOR_H_

#include "blocks/block.h"
#include "utils.h"

namespace cephalon {
    class Generator {
    public:
        const Block& operator()(Vec3i pos) const {
            if (pos.y == 0)
                return blocks::kStone;
            if (pos.y >= 1 && pos.y <= 4)
                return blocks::kDirt;
            if (pos.y == 5)
                return blocks::kGrass;
            return blocks::kAir;
        }
    };
}

#endif // CEPHALON_GENERATOR_H_