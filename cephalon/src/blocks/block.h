#ifndef CEPHALON_BLOCK_H_
#define CEPHALON_BLOCK_H_

namespace cephalon {
    class Block {
    public:
        virtual ~Block() = default;

        virtual bool isAir() const {
            return false;
        }
    };
}

#include "air.h"
#include "dirt.h"

namespace cephalon {
    namespace blocks {
        extern const Air kAir;
        extern const Dirt kDirt;
    }
}

#endif // CEPHALON_BLOCK_H_