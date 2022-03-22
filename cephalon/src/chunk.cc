#include "chunk.h"
using namespace cephalon;

#include <iterator>

#include "utils.h"
#include "world.h"

bgfx::VertexLayout Chunk::layout_;
bgfx::ProgramHandle Chunk::program_;
bgfx::TextureHandle Chunk::atlas_;
bgfx::UniformHandle Chunk::s_atlas_;

void Chunk::init() {
    layout_.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 1, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, true)
    .end();
    program_ = LoadProgram("vs_chunks", "fs_chunks");
    atlas_ = LoadTexture("textures/atlas.png", BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT);
    s_atlas_ = bgfx::createUniform("s_atlas", bgfx::UniformType::Sampler);
}

void Chunk::cleanup() {
    bgfx::destroy(s_atlas_);
    bgfx::destroy(atlas_);
    bgfx::destroy(program_);
}

Chunk::Chunk(World& world, glm::ivec2 region)
    : world_(world)
{
    region_ = region;
    dirty_ = false;
    vertex_buffer_ = bgfx::createDynamicVertexBuffer(0u, layout_, BGFX_BUFFER_ALLOW_RESIZE);
    index_buffer_ = bgfx::createDynamicIndexBuffer(0u, BGFX_BUFFER_ALLOW_RESIZE);
}

Chunk::Chunk(Chunk&& rhs) noexcept 
    : world_(rhs.world_)
{
    region_ = rhs.region_;
    dirty_ = rhs.dirty_;
    vertex_buffer_ = std::exchange(rhs.vertex_buffer_, bgfx::createDynamicVertexBuffer(0u, layout_));
    index_buffer_= std::exchange(rhs.index_buffer_, bgfx::createDynamicIndexBuffer(0u));
}

Chunk::~Chunk() noexcept {
    bgfx::destroy(vertex_buffer_);
    bgfx::destroy(index_buffer_);
}

void Chunk::setBlock(glm::ivec3 offset, const Block& block) {
    assert(offset.x >= 0 && offset.x < kVolume.x);
    assert(offset.y >= 0 && offset.y < kVolume.y);
    assert(offset.z >= 0 && offset.z < kVolume.z);
    if (blocks_[offset.x][offset.y][offset.z] != &block) {
        dirty_ = true;
        blocks_[offset.x][offset.y][offset.z] = &block;
    }
}

const Block& Chunk::getBlock(glm::ivec3 offset) const {
    assert(offset.x >= 0 && offset.x < kVolume.x);
    assert(offset.y >= 0 && offset.y < kVolume.y);
    assert(offset.z >= 0 && offset.z < kVolume.z);
    return *blocks_[offset.x][offset.y][offset.z];
}

void Chunk::rebuild() {
    std::vector<Vertex> vertices;
    std::vector<std::uint16_t> indices;

    for (int x = 0; x < kVolume.x; ++x) {
        for (int y = 0; y < kVolume.y; ++y) {
            for (int z = 0; z < kVolume.z; ++z) {
                auto& block = getBlock(glm::ivec3(x, y, z));
                if (!block.isAir()) {
                    auto pos = World::getPosition(region_, glm::ivec3(x, y, z));

                    // right
                    if (x == kVolume.x - 1 || getBlock(glm::ivec3(x + 1, y, z)).isAir()) {
                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(1, 0, -1), pos + glm::ivec3(1, -1, 0), pos + glm::ivec3(1, -1, -1)),
                            vertexAO(pos + glm::ivec3(1, 0,  1), pos + glm::ivec3(1, -1, 0), pos + glm::ivec3(1, -1,  1)),
                            vertexAO(pos + glm::ivec3(1, 0, -1), pos + glm::ivec3(1,  1, 0), pos + glm::ivec3(1,  1, -1)),
                            vertexAO(pos + glm::ivec3(1, 0,  1), pos + glm::ivec3(1,  1, 0), pos + glm::ivec3(1,  1,  1)),
                        };

                        Vertex block_vertices[] = {
                            { glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), block_ao[0], block.getTexCoord() + glm::vec2(0.0f,  0.0f) },
                            { glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), block_ao[1], block.getTexCoord() + glm::vec2(0.0f,  1.0f) },
                            { glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), block_ao[2], block.getTexCoord() + glm::vec2(0.25f, 0.0f) },
                            { glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), block_ao[3], block.getTexCoord() + glm::vec2(0.25f, 1.0f) }
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
                    if (x == 0 || getBlock(glm::ivec3(x - 1, y, z)).isAir()) {
                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(-1, 0, -1), pos + glm::ivec3(-1, -1, 0), pos + glm::ivec3(-1, -1, -1)),
                            vertexAO(pos + glm::ivec3(-1, 0,  1), pos + glm::ivec3(-1, -1, 0), pos + glm::ivec3(-1, -1,  1)),
                            vertexAO(pos + glm::ivec3(-1, 0, -1), pos + glm::ivec3(-1,  1, 0), pos + glm::ivec3(-1,  1, -1)),
                            vertexAO(pos + glm::ivec3(-1, 0,  1), pos + glm::ivec3(-1,  1, 0), pos + glm::ivec3(-1,  1,  1)),
                        };
                        
                        Vertex block_vertices[] = {
                            { glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), block_ao[0], block.getTexCoord() + glm::vec2(0.0f,  0.0f) },
                            { glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), block_ao[1], block.getTexCoord() + glm::vec2(0.0f,  1.0f) },
                            { glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), block_ao[2], block.getTexCoord() + glm::vec2(0.25f, 0.0f) },
                            { glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), block_ao[3], block.getTexCoord() + glm::vec2(0.25f, 1.0f) }
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
                    if (y == kVolume.y - 1 || getBlock(glm::ivec3(x, y + 1, z)).isAir()) {
                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(0,  1, -1), pos + glm::ivec3(-1, 1, 0), pos + glm::ivec3(-1, 1, -1)),
                            vertexAO(pos + glm::ivec3(0,  1,  1), pos + glm::ivec3(-1, 1, 0), pos + glm::ivec3(-1, 1,  1)),
                            vertexAO(pos + glm::ivec3(0,  1, -1), pos + glm::ivec3( 1, 1, 0), pos + glm::ivec3( 1, 1, -1)),
                            vertexAO(pos + glm::ivec3(0,  1,  1), pos + glm::ivec3( 1, 1, 0), pos + glm::ivec3( 1, 1,  1)),
                        };

                        Vertex block_vertices[] = {
                            { glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), block_ao[0], block.getTexCoord() + glm::vec2(0.0f,  0.0f) },
                            { glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), block_ao[1], block.getTexCoord() + glm::vec2(0.0f,  1.0f) },
                            { glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), block_ao[2], block.getTexCoord() + glm::vec2(0.25f, 0.0f) },
                            { glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), block_ao[3], block.getTexCoord() + glm::vec2(0.25f, 1.0f) }
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
                    if (y == 0 || getBlock(glm::ivec3(x, y - 1, z)).isAir()) {
                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(0, -1, -1), pos + glm::ivec3(-1, -1, 0), pos + glm::ivec3(-1, -1, -1)),
                            vertexAO(pos + glm::ivec3(0, -1,  1), pos + glm::ivec3(-1, -1, 0), pos + glm::ivec3(-1, -1,  1)),
                            vertexAO(pos + glm::ivec3(0, -1, -1), pos + glm::ivec3( 1, -1, 0), pos + glm::ivec3( 1, -1, -1)),
                            vertexAO(pos + glm::ivec3(0, -1,  1), pos + glm::ivec3( 1, -1, 0), pos + glm::ivec3( 1, -1,  1)),
                        };

                        Vertex block_vertices[] = {
                            { glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), block_ao[0], block.getTexCoord() + glm::vec2(0.0f,  0.0f) },
                            { glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), block_ao[1], block.getTexCoord() + glm::vec2(0.0f,  1.0f) },
                            { glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), block_ao[2], block.getTexCoord() + glm::vec2(0.25f, 0.0f) },
                            { glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), block_ao[3], block.getTexCoord() + glm::vec2(0.25f, 1.0f) }
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
                    if (z == kVolume.z - 1 || getBlock(glm::ivec3(x, y, z + 1)).isAir()) {
                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(0, -1, 1), pos + glm::ivec3(-1, 0, 1), pos + glm::ivec3(-1, -1, 1)),
                            vertexAO(pos + glm::ivec3(0,  1, 1), pos + glm::ivec3(-1, 0, 1), pos + glm::ivec3(-1,  1, 1)),
                            vertexAO(pos + glm::ivec3(0, -1, 1), pos + glm::ivec3( 1, 0, 1), pos + glm::ivec3( 1, -1, 1)),
                            vertexAO(pos + glm::ivec3(0,  1, 1), pos + glm::ivec3( 1, 0, 1), pos + glm::ivec3( 1,  1, 1)),
                        };

                        Vertex block_vertices[] = {
                            { glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), block_ao[0], block.getTexCoord() + glm::vec2(0.0f,  0.0f) },
                            { glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), block_ao[1], block.getTexCoord() + glm::vec2(0.0f,  1.0f) },
                            { glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), block_ao[2], block.getTexCoord() + glm::vec2(0.25f, 0.0f) },
                            { glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), block_ao[3], block.getTexCoord() + glm::vec2(0.25f, 1.0f) }
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
                    if (z == 0 || getBlock(glm::ivec3(x, y, z - 1)).isAir()) {
                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(0, -1, -1), pos + glm::ivec3(-1, 0, -1), pos + glm::ivec3(-1, -1, -1)),
                            vertexAO(pos + glm::ivec3(0,  1, -1), pos + glm::ivec3(-1, 0, -1), pos + glm::ivec3(-1,  1, -1)),
                            vertexAO(pos + glm::ivec3(0, -1, -1), pos + glm::ivec3( 1, 0, -1), pos + glm::ivec3( 1, -1, -1)),
                            vertexAO(pos + glm::ivec3(0,  1, -1), pos + glm::ivec3( 1, 0, -1), pos + glm::ivec3( 1,  1, -1)),
                        };

                        Vertex block_vertices[] = {
                            { glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), block_ao[0], block.getTexCoord() + glm::vec2(0.0f,  0.0f) },
                            { glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), block_ao[1], block.getTexCoord() + glm::vec2(0.0f,  1.0f) },
                            { glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), block_ao[2], block.getTexCoord() + glm::vec2(0.25f, 0.0f) },
                            { glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), block_ao[3], block.getTexCoord() + glm::vec2(0.25f, 1.0f) }
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

void Chunk::render() {
    bgfx::setTexture(0, s_atlas_, atlas_);
    bgfx::setVertexBuffer(0, vertex_buffer_);
    bgfx::setIndexBuffer(index_buffer_);
    bgfx::submit(0, program_);
}

float Chunk::vertexAO(glm::ivec3 side1, glm::ivec3 side2, glm::ivec3 corner) {
    auto s1 = !world_.getBlock(side1).isAir();
    auto s2 = !world_.getBlock(side2).isAir();
    auto c = !world_.getBlock(corner).isAir();
    if (s1 && s2)
        return 0.0f;
    return 1.0f - (s1 + s2 + c) / 3.0f;
}