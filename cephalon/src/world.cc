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
    for (int x = playerOffset.x - Config::kViewDistance; x <= playerOffset.x + Config::kViewDistance; ++x) {
        for (int z = playerOffset.z - Config::kViewDistance; z <= playerOffset.z + Config::kViewDistance; ++z) {
            if (chunks_.find(Vec3i(x, 0, z)) == chunks_.cend() && loadCount < Config::kChunkLoadLimit)
            {
                generator_(Vec3i(x, 0, z), chunks_[Vec3i(x, 0, z)]);
                ++loadCount;
            }
        }
    }
}

void World::render(bgfx::ProgramHandle program) {
    int rebuildCount = 0;
    for (auto& [pos, chunk] : chunks_) {
        float transform[16];
        bx::mtxTranslate(transform, pos.x * Chunk::kChunkSize.x, pos.y * Chunk::kChunkSize.y, pos.z * Chunk::kChunkSize.z);
        bgfx::setTransform(transform);
        if (chunk.needRebuild() && rebuildCount < Config::kChunkRebuildLimit) {
            chunk.rebuild();
            ++rebuildCount;
        }
        chunk.render(program);
    }
}

std::vector<AABB> World::getBoundingBoxes(AABB range) {
    std::vector<AABB> result;
    for (auto x = range.min.x; x <= range.max.x; ++x) {
        for (auto y = range.min.y; y <= range.max.y; ++y) {
            for (auto z = range.min.z; z <= range.max.z; ++z) {
                auto block = getBlock(Vec3i(x, y, z));
                if (block)
                    result.push_back(block->getBoundingBox(Vec3i(x, y, z)));
            }
        }
    }
    return result;
}