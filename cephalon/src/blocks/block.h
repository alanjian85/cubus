#ifndef CEPHALON_BLOCKS_BLOCK_H_
#define CEPHALON_BLOCKS_BLOCK_H_

#include <limits>
#include <map>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "utils/aabb.h"
#include "utils/assets.h"

namespace cephalon {
    class Block {
    public:
        static void init();

        static void deinit();

        static const Block* getBlock(const std::string& name) {
            auto it = blocks_.find(name);
            if (it != blocks_.cend())
                return it->second;
            return nullptr;
        }

        static bgfx::TextureHandle getTexture() {
            return texture_;
        }

        static std::uint16_t loadTile(const std::string& name);

        Block(const std::string& name) {
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

        virtual std::uint16_t getRightLayer() const = 0;

        virtual std::uint16_t getLeftLayer() const = 0;

        virtual std::uint16_t getTopLayer() const = 0;

        virtual std::uint16_t getBottomLayer() const = 0;

        virtual std::uint16_t getBackLayer() const = 0;

        virtual std::uint16_t getFrontLayer() const = 0;
    private:
        static std::map<std::string, const Block*> blocks_;
        static bgfx::TextureHandle texture_;
        static std::vector<std::string> texture_names_;

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