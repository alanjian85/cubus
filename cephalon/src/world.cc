#include "world.h"
using namespace cephalon;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void World::update(glm::vec3 playerPos) {
    auto playerRegion = getRegion(glm::ivec3(playerPos));

    for (auto i = chunks_.begin(); i != chunks_.end();) {
        auto& [region, chunk] = *i;
        if (std::abs(playerRegion.x - region.x) > Config::kViewDistance ||
            std::abs(playerRegion.z - region.z) > Config::kViewDistance)
        {
            chunks_.erase(i++);
        } else {
            ++i;
        }
    }
    
    int loadCount = 0;
    for (auto x = playerRegion.x - Config::kViewDistance; x <= playerRegion.x + Config::kViewDistance; ++x) {
        for (auto z = playerRegion.z - Config::kViewDistance; z <= playerRegion.z + Config::kViewDistance; ++z) {
            if (loadCount < Config::kChunkLoadLimit && chunks_.find(glm::ivec3(x, 0, z)) == chunks_.cend())
            {
                loadChunk(glm::ivec3(x, 0, z), chunks_[glm::ivec3(x, 0, z)]);
                ++loadCount;
            }
        }
    }
}

void World::render() {
    int rebuildCount = 0;
    for (auto& [pos, chunk] : chunks_) {
        auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos * Chunk::kVolume));
        bgfx::setTransform(glm::value_ptr(transform));
        if (chunk.isDirty() && rebuildCount < Config::kChunkRebuildLimit) {
            chunk.rebuild();
            ++rebuildCount;
        }
        chunk.render();
    }
}

std::vector<std::pair<glm::ivec3, AABB>> World::getBoundingBoxes(AABB range) {
    range.min.y = std::max<float>(range.min.y, 0.0f);
    range.max.y = std::min<float>(range.max.y, Chunk::kVolume.y);

    std::vector<std::pair<glm::ivec3, AABB>> result;
    for (auto x = range.min.x; x <= range.max.x; ++x) {
        for (auto y = range.min.y; y <= range.max.y; ++y) {
            for (auto z = range.min.z; z <= range.max.z; ++z) {
                auto block = getBlock(glm::ivec3(x, y, z));
                if (block && !block->isAir())
                    result.emplace_back(glm::ivec3(x, y, z), block->getBoundingBox(glm::ivec3(x, y, z)));
            }
        }
    }
    return result;
}

void World::loadChunk(glm::ivec3 region, Chunk& chunk) {
    for (int x = 0; x < Chunk::kVolume.x; ++x) {
        for (int y = 0; y < Chunk::kVolume.y; ++y) {
            for (int z = 0; z < Chunk::kVolume.z; ++z) {
                auto pos = region * Chunk::kVolume + glm::ivec3(x, y, z);
                chunk.setBlock(glm::ivec3(x, y, z), generator_(glm::ivec3(x, y, z)));
            }
        }
    }
}