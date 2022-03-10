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
    auto index = z * kChunkSizeX * kChunkSizeY + y * kChunkSizeX + x;
    if (blocks_[index] != &block) {
        dirty_ = true;
        blocks_[index] = &block;
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
        std::vector<Vertex> vertices;
        std::vector<std::uint16_t> indices;

        for (int x = 0; x < kChunkSizeX; x++) {
            for (int y = 0; y < kChunkSizeY; y++) {
                for (int z = 0; z < kChunkSizeZ; ++z) {
                    auto block = blocks_[z * kChunkSizeX * kChunkSizeY + y * kChunkSizeX + x];
                    updateBlock(block, vertices, indices);
                }
            }
        }

        vbh_ = bgfx::createVertexBuffer(bgfx::copy(vertices.data(), vertices.size() * sizeof(Vertex)), Vertex::layout);
        ibh_ = bgfx::createIndexBuffer(bgfx::copy(indices.data(), indices.size() * sizeof(std::uint16_t)));

        dirty_ = false;
    }
}

void Chunk::updateBlock(const Block* block, std::vector<Vertex>& vertices, std::vector<std::uint16_t>& indices) {
    if (!block->isAir()) {
        
    }
}