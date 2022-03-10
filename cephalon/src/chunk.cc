#include "chunk.h"
using namespace cephalon;

bgfx::VertexLayout Vertex::layout;

Chunk::Chunk(int x, int y, int z)
{
    x_ = x;
    y_ = y;
    z_ = z;
    dirty_ = true;
    blocks_.resize(kChunkSizeX * kChunkSizeY * kChunkSizeZ, &blocks::kAir);
}

void Chunk::setPosition(int x, int y, int z) {
    if (x_ != x || y_ != y || z_ != z) {
        x_ = x;
        y_ = y;
        z_ = z;
        dirty_ = true;
    }
}

void Chunk::setBlock(int x, int y, int z, const Block& block) {
    x -= x_;
    y -= y_;
    z -= z_;
    assert(x >= 0 && x < kChunkSizeX);
    assert(y >= 0 && y < kChunkSizeY);
    assert(z >= 0 && z < kChunkSizeZ);
    auto& dest = blocks_[z * kChunkSizeX * kChunkSizeY + y * kChunkSizeX + x];
    if (dest != &block) {
        dirty_ = true;
        dest = &block;
    }
}

const Block& Chunk::getBlock(int x, int y, int z) const {
    x -= x_;
    y -= y_;
    z -= z_;
    assert(x >= 0 && x < kChunkSizeX);
    assert(y >= 0 && y < kChunkSizeY);
    assert(z >= 0 && z < kChunkSizeZ);
    return *blocks_[z * kChunkSizeX * kChunkSizeY + y * kChunkSizeX + x];
}

void Chunk::update() {
    if (dirty_) {
        Vertex vertices[1];
        std::uint16_t indices[1];

        vbh_ = bgfx::createVertexBuffer(bgfx::copy(vertices, sizeof(vertices)), Vertex::layout);
        ibh_ = bgfx::createIndexBuffer(bgfx::copy(indices, sizeof(indices)));
    }
}