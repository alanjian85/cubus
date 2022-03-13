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
            else if (pos.y == 1)
                return blocks::kDirt;
            else if (pos.y == 2)
                return blocks::kGrass;
            else
                return blocks::kAir;
        }
    };
}

#endif // CEPHALON_GENERATOR_H_