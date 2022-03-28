#ifndef CEPHALON_BLOCKS_BLOCK_H_
#define CEPHALON_BLOCKS_BLOCK_H_

#include <limits>
#include <map>
#include <string>

#include <glm/glm.hpp>

#include "utils/aabb.h"
#include "utils/assets.h"
#include "utils/atlas.h"

namespace cephalon {
    class Block {
    public:
        static void init(Atlas& atlas);

        static const Block* getBlock(const std::string& name) {
            auto it = blocks_.find(name);
            if (it != blocks_.cend())
                return it->second;
            return nullptr;
        }

        Block(const char* name) {
            blocks_[name] = this;
            name_ = name;
        }

        virtual ~Block() noexcept = default;

        virtual bool isAir() const {
            return false;
        }

        virtual AABB getBoundingBox(glm::ivec3 pos) const {
            return AABB(glm::vec3(pos) - 0.5f, glm::vec3(pos) + 0.5f);
        }

        const std::string& getName() const {
            return name_;
        }

        virtual Region getRightRegion() const = 0;

        virtual Region getLeftRegion() const = 0;

        virtual Region getTopRegion() const = 0;

        virtual Region getBottomRegion() const = 0;

        virtual Region getBackRegion() const = 0;

        virtual Region getFrontRegion() const = 0;
    private:
        static std::map<std::string, const Block*> blocks_;
    
        std::string name_;
    };
}

#include "air.h"
#include "dirt.h"
#include "grass.h"
#include "sand.h"
#include "stone.h"
#include "wood.h"

namespace cephalon {
    namespace blocks {
        extern Air air;
        extern Dirt dirt;
        extern Grass grass;
        extern Sand sand;
        extern Stone stone;
        extern Wood wood;
    }
}

#endif // CEPHALON_BLOCKS_BLOCK_H_