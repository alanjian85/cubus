#include "chunk.h"
using namespace cephalon;

#include <iterator>

bgfx::VertexLayout Vertex::layout;

Chunk::Chunk()
{
    dirty_ = true;
    blocks_.resize(kChunkSize.x * kChunkSize.y * kChunkSize.z, &blocks::kAir);
    vertex_buffer_ = bgfx::createDynamicVertexBuffer(1, Vertex::layout, BGFX_BUFFER_ALLOW_RESIZE);
    index_buffer_ = bgfx::createDynamicIndexBuffer(1, BGFX_BUFFER_ALLOW_RESIZE);
}

void Chunk::setBlock(Vec3i pos, const Block& block) {
    assert(pos.x >= 0 && pos.x < kChunkSize.x);
    assert(pos.y >= 0 && pos.y < kChunkSize.y);
    assert(pos.z >= 0 && pos.z < kChunkSize.z);
    auto index = pos.z * kChunkSize.x * kChunkSize.y + pos.y * kChunkSize.x + pos.x;
    if (blocks_[index] != &block) {
        dirty_ = true;
        blocks_[index] = &block;
    }
}

const Block& Chunk::getBlock(Vec3i pos) const {
    assert(pos.x >= 0 && pos.x < kChunkSize.x);
    assert(pos.y >= 0 && pos.y < kChunkSize.y);
    assert(pos.z >= 0 && pos.z < kChunkSize.z);
    return *blocks_[pos.z * kChunkSize.x * kChunkSize.y + pos.y * kChunkSize.x + pos.x];
}

void Chunk::update() {
    if (dirty_) {
        std::vector<Vertex> vertices;
        std::vector<std::uint16_t> indices;

        for (int x = 0; x < kChunkSize.x; ++x) {
            for (int y = 0; y < kChunkSize.y; ++y) {
                for (int z = 0; z < kChunkSize.z; ++z) {
                    auto& block = getBlock({x, y, z});
                    if (!block.isAir()) {
                        auto color = block.getColor();

                        auto fx = static_cast<float>(x);
                        auto fy = static_cast<float>(y);
                        auto fz = static_cast<float>(z);

                        // right
                        if (x == kChunkSize.x - 1 || getBlock({x + 1, y, z}).isAir()) {
                            Vertex block_vertices[] = {
                                { fx + 1, fy    , fz    , color },
                                { fx + 1, fy    , fz + 1, color },
                                { fx + 1, fy + 1, fz    , color },
                                { fx + 1, fy + 1, fz + 1, color }
                            };

                            auto index_base = static_cast<int>(vertices.size());
                            int block_indices[] = {
                                index_base + 1, index_base + 3, index_base + 2,
                                index_base + 1, index_base + 2, index_base + 0
                            };

                            vertices.insert(vertices.cend(), std::cbegin(block_vertices), std::cend(block_vertices));
                            indices.insert(indices.cend(), std::cbegin(block_indices), std::cend(block_indices));
                        }

                        // left
                        if (x == 0 || getBlock({x - 1, y, z}).isAir()) {
                            Vertex block_vertices[] = {
                                { fx    , fy    , fz    , color },
                                { fx    , fy    , fz + 1, color },
                                { fx    , fy + 1, fz    , color },
                                { fx    , fy + 1, fz + 1, color }
                            };

                            auto index_base = static_cast<int>(vertices.size());
                            int block_indices[] = {
                                index_base + 0, index_base + 2, index_base + 3,
                                index_base + 0, index_base + 3, index_base + 1,
                            };

                            vertices.insert(vertices.cend(), std::cbegin(block_vertices), std::cend(block_vertices));
                            indices.insert(indices.cend(), std::cbegin(block_indices), std::cend(block_indices));
                        }

                        // top
                        if (y == kChunkSize.y - 1 || getBlock({x, y + 1, z}).isAir()) {
                            Vertex block_vertices[] = {
                                { fx    , fy + 1, fz    , color },
                                { fx    , fy + 1, fz + 1, color },
                                { fx + 1, fy + 1, fz    , color },
                                { fx + 1, fy + 1, fz + 1, color }
                            };

                            auto index_base = static_cast<int>(vertices.size());
                            int block_indices[] = {
                                index_base + 2, index_base + 3, index_base + 1,
                                index_base + 2, index_base + 1, index_base + 0,
                            };

                            vertices.insert(vertices.cend(), std::cbegin(block_vertices), std::cend(block_vertices));
                            indices.insert(indices.cend(), std::cbegin(block_indices), std::cend(block_indices));
                        }

                        // bottom
                        if (y == 0 || getBlock({x, y - 1, z}).isAir()) {
                            Vertex block_vertices[] = {
                                { fx    , fy    , fz    , color },
                                { fx    , fy    , fz + 1, color },
                                { fx + 1, fy    , fz    , color },
                                { fx + 1, fy    , fz + 1, color },
                            };

                            auto index_base = static_cast<int>(vertices.size());
                            int block_indices[] = {
                                index_base + 3, index_base + 2, index_base + 0,
                                index_base + 3, index_base + 0, index_base + 1,
                            };

                            vertices.insert(vertices.cend(), std::cbegin(block_vertices), std::cend(block_vertices));
                            indices.insert(indices.cend(), std::cbegin(block_indices), std::cend(block_indices));
                        }

                        // back
                        if (z == kChunkSize.z - 1 || getBlock({x, y, z + 1}).isAir()) {
                            Vertex block_vertices[] = {
                                { fx    , fy    , fz + 1, color },
                                { fx    , fy + 1, fz + 1, color },
                                { fx + 1, fy    , fz + 1, color },
                                { fx + 1, fy + 1, fz + 1, color }
                            };

                            auto index_base = static_cast<int>(vertices.size());
                            int block_indices[] = {
                                index_base + 1, index_base + 3, index_base + 2,
                                index_base + 0, index_base + 1, index_base + 2,
                            };

                            vertices.insert(vertices.cend(), std::cbegin(block_vertices), std::cend(block_vertices));
                            indices.insert(indices.cend(), std::cbegin(block_indices), std::cend(block_indices));
                        }

                        // front
                        if (z == 0 || getBlock({x, y, z - 1}).isAir()) {
                            Vertex block_vertices[] = {
                                { fx    , fy    , fz    , color },
                                { fx    , fy + 1, fz    , color },
                                { fx + 1, fy    , fz    , color },
                                { fx + 1, fy + 1, fz    , color },
                            };

                            auto index_base = static_cast<int>(vertices.size());
                            int block_indices[] = {
                                index_base + 2, index_base + 3, index_base + 1,
                                index_base + 2, index_base + 1, index_base + 0
                            };

                            vertices.insert(vertices.cend(), std::cbegin(block_vertices), std::cend(block_vertices));
                            indices.insert(indices.cend(), std::cbegin(block_indices), std::cend(block_indices));
                        }
                    }
                }
            }
        }

        if (!vertices.empty()) {
            bgfx::update(vertex_buffer_, 0, bgfx::copy(vertices.data(), vertices.size() * sizeof(Vertex)));
            bgfx::update(index_buffer_, 0, bgfx::copy(indices.data(), indices.size() * sizeof(std::uint16_t)));
        }
        dirty_ = false;
    }
}