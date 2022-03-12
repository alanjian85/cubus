#include "world.h"
using namespace cephalon;

void World::render(bgfx::ProgramHandle program) {
    for (auto& [pos, chunk] : chunks_) {
        float transform[16];
        bx::mtxTranslate(transform, pos.x * Chunk::kChunkSize.x, pos.y * Chunk::kChunkSize.y, pos.z * Chunk::kChunkSize.z);
        bgfx::setTransform(transform);
        bgfx::setVertexBuffer(0, chunk.getVertexBuffer());
        bgfx::setIndexBuffer(chunk.getIndexBuffer());
        bgfx::submit(0, program);
    }
}