#include "world.h"
using namespace cephalon;

#include <iostream>

void World::update(bx::Vec3 playerPos) {
    auto playerOffset = getChunkOffset({
        static_cast<int>(playerPos.x), 
        static_cast<int>(playerPos.y), 
        static_cast<int>(playerPos.z)
    });

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
            if (chunks_.find({x, 0, z}) == chunks_.cend() &&
                loadCount < Config::kMaxLoadCount)
            {
                generator_({x, 0, z}, chunks_[{x, 0, z}]);
                ++loadCount;
            }
        }
    }
}

void World::render(bgfx::ProgramHandle program) {
    for (auto& [pos, chunk] : chunks_) {
        float transform[16];
        bx::mtxTranslate(transform, pos.x * Chunk::kChunkSize.x, pos.y * Chunk::kChunkSize.y, pos.z * Chunk::kChunkSize.z);
        bgfx::setTransform(transform);
        chunk.updateMesh();
        chunk.render(program);
    }
}