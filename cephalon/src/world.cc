#include "world.h"
using namespace cephalon;

void World::render(bgfx::ProgramHandle program) {
    for (auto& [_, chunk] : chunks_) {
        bgfx::setVertexBuffer(0, chunk.getVertexBuffer());
        bgfx::setIndexBuffer(chunk.getIndexBuffer());
        bgfx::submit(0, program);
    }
}