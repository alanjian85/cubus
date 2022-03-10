#ifndef CEPHALON_BLOCK_H_
#define CEPHALON_BLOCK_H_

#include <cstdint>

namespace cephalon {
    class Block {
    public:
        virtual ~Block() = default;

        virtual bool isAir() const {
            return false;
        }

        virtual std::uint32_t getColor() const = 0;
    };
}

#include "air.h"
#include "dirt.h"
#include "stone.h"

namespace cephalon {
    namespace blocks {
        extern const Air kAir;
        extern const Dirt kDirt;
        extern const Stone kStone;
    }
}

#endif // CEPHALON_BLOCK_H_