#include "world.h"
using namespace cephalon;

#include <iostream>

void World::render(bgfx::ProgramHandle program) {
    for (auto& [pos, chunk] : chunks_) {
        if (!chunk.isEmpty()) {
            float transform[16];
            bx::mtxTranslate(transform, pos.x * Chunk::kChunkSize.x, pos.y * Chunk::kChunkSize.y, pos.z * Chunk::kChunkSize.z);
            bgfx::setTransform(transform);
            chunk.render(program);
        }
    }
}