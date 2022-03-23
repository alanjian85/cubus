#include "chunk.h"
using namespace cephalon;

#include <iterator>

#include "utils/assets.h"
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
    for (int x = 0; x < kVolume.x; ++x) {
        for (int y = 0; y < kVolume.y; ++y) {
            for (int z = 0; z < kVolume.z; ++z) {
                blocks_[x][y][z] = &blocks::kAir;
            }
        }
    }
    vertex_buffer_ = bgfx::createDynamicVertexBuffer(0u, layout_, BGFX_BUFFER_ALLOW_RESIZE);
    index_buffer_ = bgfx::createDynamicIndexBuffer(0u, BGFX_BUFFER_ALLOW_RESIZE);
}

Chunk::Chunk(Chunk&& rhs) noexcept 
    : world_(rhs.world_)
{
    region_ = rhs.region_;
    dirty_ = rhs.dirty_;
    for (int x = 0; x < kVolume.x; ++x) {
        for (int y = 0; y < kVolume.y; ++y) {
            for (int z = 0; z < kVolume.z; ++z) {
                blocks_[x][y][z] = rhs.blocks_[x][y][z];
            }
        }
    }
    vertex_buffer_ = std::exchange(rhs.vertex_buffer_, BGFX_INVALID_HANDLE);
    index_buffer_= std::exchange(rhs.index_buffer_, BGFX_INVALID_HANDLE);
}

Chunk::~Chunk() noexcept {
    if (vertex_buffer_.idx != bgfx::kInvalidHandle) {
        bgfx::destroy(vertex_buffer_);
        bgfx::destroy(index_buffer_);
    }
}

void Chunk::setBlock(glm::ivec3 offset, const Block& block) {
    assert(offset.x >= 0 && offset.x < kVolume.x);
    assert(offset.y >= 0 && offset.y < kVolume.y);
    assert(offset.z >= 0 && offset.z < kVolume.z);
    if (blocks_[offset.x][offset.y][offset.z] != &block) {
        dirty_ = true;
        blocks_[offset.x][offset.y][offset.z] = &block;

        if (offset.x == 0) {
            if (auto chunk = world_.getChunk(region_ + glm::ivec2(-1,  0)))
                chunk->dirty_ = true;
        }
        if (offset.x == kVolume.x - 1) {
            if (auto chunk = world_.getChunk(region_ + glm::ivec2( 1,  0)))
                chunk->dirty_ = true;
        }
        if (offset.z == 0) {
            if (auto chunk = world_.getChunk(region_ + glm::ivec2( 0, -1)))
                chunk->dirty_ = true;
        }
        if (offset.z == kVolume.z - 1) {
            if (auto chunk = world_.getChunk(region_ + glm::ivec2( 0,  1)))
                chunk->dirty_ = true;
        }
        if (offset.x == 0 && offset.z == 0) {
            if (auto chunk = world_.getChunk(region_ + glm::ivec2(-1, -1)))
                chunk->dirty_ = true;
        }
        if (offset.x == 0 && offset.z == kVolume.z - 1) {
            if (auto chunk = world_.getChunk(region_ + glm::ivec2(-1,  1)))
                chunk->dirty_ = true;
        }
        if (offset.x == kVolume.x - 1 && offset.z == 0) {
            if (auto chunk = world_.getChunk(region_ + glm::ivec2( 1, -1)))
                chunk->dirty_ = true;
        }
        if (offset.x == kVolume.x - 1 && offset.z == kVolume.z - 1) {
            if (auto chunk = world_.getChunk(region_ + glm::ivec2( 1,  1)))
                chunk->dirty_ = true;
        }
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
                    if (world_.getBlock(pos + glm::ivec3(1, 0, 0)).isAir()) {
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
                    if (world_.getBlock(pos + glm::ivec3(-1, 0, 0)).isAir()) {
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
                    if (world_.getBlock(pos + glm::ivec3(0, 1, 0)).isAir()) {
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
                    if (world_.getBlock(pos + glm::ivec3(0, -1, 0)).isAir()) {
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
                    if (world_.getBlock(pos + glm::ivec3(0, 0, 1)).isAir()) {
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
                    if (world_.getBlock(pos + glm::ivec3(0, 0, -1)).isAir()) {
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

void Chunk::render() const {
    bgfx::setTexture(0, s_atlas_, atlas_);
    bgfx::setVertexBuffer(0, vertex_buffer_);
    bgfx::setIndexBuffer(index_buffer_);
    bgfx::submit(0, program_);
}

float Chunk::vertexAO(glm::ivec3 side1, glm::ivec3 side2, glm::ivec3 corner) const {
    auto s1 = !world_.getBlock(side1).isAir();
    auto s2 = !world_.getBlock(side2).isAir();
    auto c = !world_.getBlock(corner).isAir();
    if (s1 && s2)
        return 0.0f;
    return 1.0f - (s1 + s2 + c) / 3.0f;
}

bool Chunk::intersect(Ray ray, float dmin, float dmax, Direction& dir, glm::ivec3& offset, float& dist) const {
    float temp_dist;
    Direction direction;
    AABB aabb(World::getPosition(region_, glm::ivec3(0)), World::getPosition(region_, kVolume - 1));
    if (aabb.intersect(ray, dmin, dmax, direction, temp_dist)) {
        bool intersected = false;
        for (int x = 0; x < kVolume.x; ++x) {
            for (int y = 0; y < kVolume.y; ++y) {
                for (int z = 0; z < kVolume.z; ++z) {
                    auto pos = World::getPosition(region_, glm::ivec3(x, y, z));
                    auto& block = getBlock(glm::ivec3(x, y, z));
                    if (!block.isAir() && block.getBoundingBox(pos).intersect(ray, dmin, dmax, direction, dmax)) {
                        auto neighbor_pos = pos + directionToVector(direction);
                        if (world_.getBlock(neighbor_pos).isAir()) {
                            intersected = true;
                            dir = direction;
                            offset = glm::ivec3(x, y, z);
                            dist = dmax;
                        }
                    }
                }
            }
        }
        return intersected;
    }
    return false;
}