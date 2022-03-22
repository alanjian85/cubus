#include "world.h"
using namespace cephalon;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void World::update(glm::vec3 player_pos) {
    auto player_region = getRegion(glm::ivec3(player_pos));

    for (auto i = chunks_.begin(); i != chunks_.end();) {
        auto& [region, chunk] = *i;
        if (std::abs(player_region.x - region.x) > Config::kViewDistance ||
            std::abs(player_region.y - region.y) > Config::kViewDistance)
        {
            chunks_.erase(i++);
        } else {
            ++i;
        }
    }
    
    int load_count = 0;
    for (auto x = player_region.x - Config::kViewDistance; x <= player_region.x + Config::kViewDistance; ++x) {
        for (auto y = player_region.y - Config::kViewDistance; y <= player_region.y + Config::kViewDistance; ++y) {
            if (load_count < Config::kChunkLoadLimit) {
                auto [chunk, loaded] = chunks_.emplace(glm::ivec2(x, y), Chunk(*this, glm::ivec2(x, y)));
                if (loaded) {
                    loadChunk(glm::ivec2(x, y), chunk->second);
                    ++load_count;
                }
            }
        }
    }
}

void World::render() {
    if (rebuild_chunks_.empty()) {
        for (auto& [region, chunk] : chunks_) {
            if (chunk.isDirty()) {
                rebuild_chunks_[region] = &chunk;
            }
        }
    }

    int rebuild_count = 0;
    for (auto it = rebuild_chunks_.begin(); it != rebuild_chunks_.end();) {
        auto& [region, chunk] = *it;
        if (rebuild_count < Config::kChunkRebuildLimit) {
            chunk->rebuild();
            rebuild_chunks_.erase(it++);
            ++rebuild_count;
        } else {
            break;
        }
    }
    
    for (auto& [region, chunk] : chunks_) {
        auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(getPosition(region, glm::ivec3(0))));
        bgfx::setTransform(glm::value_ptr(transform));
        chunk.render();
    }
}

std::vector<std::pair<glm::ivec3, AABB>> World::getBoundingBoxes(AABB range) {
    std::vector<std::pair<glm::ivec3, AABB>> result;
    for (auto x = range.min.x; x <= range.max.x; ++x) {
        for (auto y = range.min.y; y <= range.max.y; ++y) {
            for (auto z = range.min.z; z <= range.max.z; ++z) {
                if (y > 0 && y < Chunk::kVolume.y) {
                    auto& block = getBlock(glm::ivec3(x, y, z));
                    if (!block.isAir())
                        result.emplace_back(glm::ivec3(x, y, z), block.getBoundingBox(glm::ivec3(x, y, z)));
                }
            }
        }
    }
    return result;
}

void World::loadChunk(glm::ivec2 region, Chunk& chunk) {
    for (int x = 0; x < Chunk::kVolume.x; ++x) {
        for (int y = 0; y < Chunk::kVolume.y; ++y) {
            for (int z = 0; z < Chunk::kVolume.z; ++z) {
                auto pos = getPosition(region, glm::ivec3(x, y, z));
                chunk.setBlock(glm::ivec3(x, y, z), generator_(pos));
            }
        }
    }
    for (auto [pos, block] : blocks_) {
        if (getRegion(pos) == region) {
            chunk.setBlock(getOffset(pos), *block);
        }
    }
}