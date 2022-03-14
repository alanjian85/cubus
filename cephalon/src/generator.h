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
            return blocks::kAir;
        }
    };
}

#endif // CEPHALON_GENERATOR_H_