#include "chunk.h"
using namespace cephalon;

#include <iterator>

bgfx::VertexLayout Vertex::layout;

Chunk::Chunk(Vec3i pos)
{
    pos_.x = pos.x;
    pos_.y = pos.y;
    pos_.z = pos.z;
    dirty_ = true;
    blocks_.resize(kChunkSize.x * kChunkSize.y * kChunkSize.z, &blocks::kAir);
    vertex_buffer_ = bgfx::createDynamicVertexBuffer(1, Vertex::layout, BGFX_BUFFER_ALLOW_RESIZE);
    index_buffer_ = bgfx::createDynamicIndexBuffer(1, BGFX_BUFFER_ALLOW_RESIZE);
}

void Chunk::setPos(Vec3i pos) {
    if (pos_.x != pos.x || pos_.y != pos.y || pos_.z != pos.z) {
        pos_.x = pos.x;
        pos_.y = pos.y;
        pos_.z = pos.z;
        dirty_ = true;
    }
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

        for (int i = 0; i < kChunkSize.x; i++) {
            for (int j = 0; j < kChunkSize.y; j++) {
                for (int k = 0; k < kChunkSize.z; ++k) {
                    auto& block = getBlock({i, j, k});
                    if (!block.isAir()) {
                        auto color = block.getColor();
                        
                        auto x = static_cast<float>(pos_.x + i);
                        auto y = static_cast<float>(pos_.y + j);
                        auto z = static_cast<float>(pos_.z + k);

                        // right
                        if (i == kChunkSize.x - 1 || getBlock({i + 1, j, k}).isAir()) {
                            Vertex block_vertices[] = {
                                { x + 1, y    , z    , color },
                                { x + 1, y    , z + 1, color },
                                { x + 1, y + 1, z    , color },
                                { x + 1, y + 1, z + 1, color }
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
                        if (i == 0 || getBlock({i - 1, j, k}).isAir()) {
                            Vertex block_vertices[] = {
                                { x    , y    , z    , color },
                                { x    , y    , z + 1, color },
                                { x    , y + 1, z    , color },
                                { x    , y + 1, z + 1, color }
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
                        if (j == kChunkSize.y - 1 || getBlock({i, j + 1, k}).isAir()) {
                            Vertex block_vertices[] = {
                                { x    , y + 1, z    , color },
                                { x    , y + 1, z + 1, color },
                                { x + 1, y + 1, z    , color },
                                { x + 1, y + 1, z + 1, color }
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
                        if (j == 0 || getBlock({i, j - 1, k}).isAir()) {
                            Vertex block_vertices[] = {
                                { x    , y    , z    , color },
                                { x    , y    , z + 1, color },
                                { x + 1, y    , z    , color },
                                { x + 1, y    , z + 1, color },
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
                        if (k == kChunkSize.z - 1 || getBlock({i, j, k + 1}).isAir()) {
                            Vertex block_vertices[] = {
                                { x    , y    , z + 1, color },
                                { x    , y + 1, z + 1, color },
                                { x + 1, y    , z + 1, color },
                                { x + 1, y + 1, z + 1, color }
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
                        if (k == 0 || getBlock({i, j, k - 1}).isAir()) {
                            Vertex block_vertices[] = {
                                { x    , y    , z    , color },
                                { x    , y + 1, z    , color },
                                { x + 1, y    , z    , color },
                                { x + 1, y + 1, z    , color },
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

        bgfx::update(vertex_buffer_, 0, bgfx::copy(vertices.data(), vertices.size() * sizeof(Vertex)));
        bgfx::update(index_buffer_, 0, bgfx::copy(indices.data(), indices.size() * sizeof(std::uint16_t)));
        dirty_ = false;
    }
}