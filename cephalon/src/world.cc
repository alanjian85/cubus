#include "world.h"
using namespace cephalon;

#include <iostream>

void World::update(bx::Vec3 playerPos) {
    auto playerOffset = getChunkOffset(playerPos);

    for (auto i = chunks_.begin(); i != chunks_.end();) {
        auto& [pos, chunk] = *i;
        if (std::abs(playerOffset.x - pos.x) > Config::kViewDistance ||
            std::abs(playerOffset.z - pos.z) > Config::kViewDistance)
        {
            chunks_.erase(i++);
        } else {
            ++i;
        }
    }
    
    int loadCount = 0;
    for (auto x = playerOffset.x - Config::kViewDistance; x <= playerOffset.x + Config::kViewDistance; ++x) {
        for (auto z = playerOffset.z - Config::kViewDistance; z <= playerOffset.z + Config::kViewDistance; ++z) {
            if (loadCount < Config::kChunkLoadLimit && chunks_.find(Vec3i(x, 0, z)) == chunks_.cend())
            {
                loadChunk(Vec3i(x, 0, z), chunks_[Vec3i(x, 0, z)]);
                ++loadCount;
            }
        }
    }
}

void World::render(bgfx::ProgramHandle program) {
    int rebuildCount = 0;
    for (auto& [pos, chunk] : chunks_) {
        float transform[16];
        bx::mtxTranslate(transform, pos.x * Chunk::kChunkSize.x, 0, pos.z * Chunk::kChunkSize.z);
        bgfx::setTransform(transform);
        if (chunk.needRebuild() && rebuildCount < Config::kChunkRebuildLimit) {
            chunk.rebuild();
            ++rebuildCount;
        }
        chunk.render(program);
    }
}

std::vector<std::pair<Vec3i, AABB>> World::getBoundingBoxes(AABB range) {
    range.min.y = std::max(range.min.y, 0);
    range.max.y = std::min(range.max.y, Chunk::kChunkSize.y);

    std::vector<std::pair<Vec3i, AABB>> result;
    for (auto x = range.min.x; x <= range.max.x; ++x) {
        for (auto y = range.min.y; y <= range.max.y; ++y) {
            for (auto z = range.min.z; z <= range.max.z; ++z) {
                auto& block = getBlock(Vec3i(x, y, z));
                if (!block.isAir())
                    result.emplace_back(Vec3i(x, y, z), block.getBoundingBox(Vec3i(x, y, z)));
            }
        }
    }
    return result;
}

void World::loadChunk(Vec3i offset, Chunk& chunk) {
    for (int x = 0; x < Chunk::kChunkSize.x; ++x) {
        for (int y = 0; y < Chunk::kChunkSize.y; ++y) {
            for (int z = 0; z < Chunk::kChunkSize.z; ++z) {
                auto pos = offset * Chunk::kChunkSize + Vec3i(x, y, z);
                chunk.setBlock(Vec3i(x, y, z), generator_(Vec3i(x, y, z)));
            }
        }
    }
    for (auto [pos, block] : blocks_) {
        if (getChunkOffset(pos) == offset) {
            chunk.setBlock(getChunkPos(pos), *block);
        }
    }
}