#include "world.h"
using namespace cephalon;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void World::update(glm::vec3 player_pos) {
    auto player_region = getRegion(glm::ivec3(player_pos));

    for (auto i = chunks_.begin(); i != chunks_.end();) {
        auto& [region, chunk] = *i;
        if (std::abs(player_region.x - region.x) > Config::kViewDistance ||
            std::abs(player_region.z - region.z) > Config::kViewDistance)
        {
            chunks_.erase(i++);
        } else {
            ++i;
        }
    }
    
    int load_count = 0;
    for (auto x = player_region.x - Config::kViewDistance; x <= player_region.x + Config::kViewDistance; ++x) {
        for (auto z = player_region.z - Config::kViewDistance; z <= player_region.z + Config::kViewDistance; ++z) {
            if (load_count < Config::kChunkLoadLimit && chunks_.find(glm::ivec3(x, 0, z)) == chunks_.cend())
            {
                loadChunk(glm::ivec3(x, 0, z), chunks_[glm::ivec3(x, 0, z)]);
                ++load_count;
            }
        }
    }
}

void World::render() {
    if (rebuild_chunks_.empty()) {
        for (auto& [region, chunk] : chunks_) {
            if (chunk.isDirty()) {
                rebuild_chunks_[region] = &chunk;
                if (auto it = chunks_.find(region + glm::ivec3( 1, 0,  0)); it != chunks_.cend()) // ( 1,  0)
                    rebuild_chunks_[region + glm::ivec3( 1, 0,  0)] = &it->second;
                if (auto it = chunks_.find(region + glm::ivec3(-1, 0,  0)); it != chunks_.cend()) // (-1,  0)
                    rebuild_chunks_[region + glm::ivec3(-1, 0,  0)] = &it->second;
                if (auto it = chunks_.find(region + glm::ivec3( 0, 0,  1)); it != chunks_.cend()) // ( 0,  1)
                    rebuild_chunks_[region + glm::ivec3( 0, 0,  1)] = &it->second;
                if (auto it = chunks_.find(region + glm::ivec3( 0, 0, -1)); it != chunks_.cend()) // ( 0, -1)
                    rebuild_chunks_[region + glm::ivec3( 0, 0, -1)] = &it->second;
            }
        }
    }

    int rebuild_count = 0;
    for (auto it = rebuild_chunks_.begin(); it != rebuild_chunks_.end();) {
        auto& [region, chunk] = *it;
        if (rebuild_count < Config::kChunkRebuildLimit) {
            chunk->rebuild(*this, region);
            rebuild_chunks_.erase(it++);
            ++rebuild_count;
        } else {
            break;
        }
    }
    
    for (auto& [region, chunk] : chunks_) {
        auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(region * Chunk::kVolume));
        bgfx::setTransform(glm::value_ptr(transform));
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
                auto& block = getBlock(glm::ivec3(x, y, z));
                if (!block.isAir())
                    result.emplace_back(glm::ivec3(x, y, z), block.getBoundingBox(glm::ivec3(x, y, z)));
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
    for (auto [pos, block] : blocks_) {
        if (getRegion(pos) == region) {
            chunk.setBlock(getChunkPos(pos), *block);
        }
    }
}