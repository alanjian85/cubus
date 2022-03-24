#include "chunk.h"
using namespace cephalon;

#include <iterator>

#include <glm/gtc/type_ptr.hpp>

#include "utils/assets.h"
#include "world.h"

bgfx::VertexLayout Chunk::layout_;
bgfx::ProgramHandle Chunk::program_;
bgfx::TextureHandle Chunk::atlas_;
bgfx::UniformHandle Chunk::u_fog_;
bgfx::UniformHandle Chunk::s_atlas_;
bgfx::UniformHandle Chunk::s_heightmap_;

void Chunk::init() {
    layout_.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 1, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color1, 1, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
    .end();
    program_ = LoadProgram("vs_chunks", "fs_chunks");
    atlas_ = LoadTexture("textures/atlas.dds", BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT);
    u_fog_ = bgfx::createUniform("u_fog", bgfx::UniformType::Vec4);
    s_atlas_ = bgfx::createUniform("s_atlas", bgfx::UniformType::Sampler);
    s_heightmap_ = bgfx::createUniform("s_heightmap", bgfx::UniformType::Sampler);
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
    heightmap_ = bgfx::createTexture2D(
        kVolume.x, kVolume.z, 
        false, 1,
        bgfx::TextureFormat::R8, 
        BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT
    );
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
    heightmap_ = std::exchange(rhs.heightmap_, BGFX_INVALID_HANDLE);
}

Chunk::~Chunk() noexcept {
    if (bgfx::isValid(vertex_buffer_)) {
        bgfx::destroy(vertex_buffer_);
        bgfx::destroy(index_buffer_);
        bgfx::destroy(heightmap_);
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
    std::uint8_t heightmap[kVolume.x][kVolume.z] = {};

    for (int x = 0; x < kVolume.x; ++x) {
        for (int y = 0; y < kVolume.y; ++y) {
            for (int z = 0; z < kVolume.z; ++z) {
                auto offset = glm::ivec3(x, y, z);
                auto& block = getBlock(offset);
                if (!block.isAir()) {
                    auto pos = World::getPosition(region_, offset);
                    glm::vec2 block_texcoord1;
                    if (bgfx::getCaps()->originBottomLeft)
                        block_texcoord1 = glm::vec2(
                            static_cast<float>(z) / kVolume.z, 
                            1.0f - static_cast<float>(x) / kVolume.x
                        );
                    else
                        block_texcoord1 = glm::vec2(
                            static_cast<float>(z) / kVolume.z, 
                            static_cast<float>(x) / kVolume.x
                        );
                    auto height = static_cast<float>(y) / kVolume.y;
                    auto iheight = static_cast<std::uint8_t>(height * 255);
                    if (iheight > heightmap[x][z]) {
                        heightmap[x][z] = iheight;
                    }

                    // right
                    if (world_.getBlock(pos + glm::ivec3(1, 0, 0)).isAir()) {
                        glm::vec3 block_pos[] = {
                            glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f),
                            glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f),
                            glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f),
                            glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f)
                        };

                        glm::vec3 block_normal[] = {
                            glm::vec3( 1.0f,  0.0f,  0.0f),
                            glm::vec3( 1.0f,  0.0f,  0.0f),
                            glm::vec3( 1.0f,  0.0f,  0.0f),
                            glm::vec3( 1.0f,  0.0f,  0.0f)
                        };

                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(1, 0, -1), pos + glm::ivec3(1, -1, 0), pos + glm::ivec3(1, -1, -1)),
                            vertexAO(pos + glm::ivec3(1, 0,  1), pos + glm::ivec3(1, -1, 0), pos + glm::ivec3(1, -1,  1)),
                            vertexAO(pos + glm::ivec3(1, 0, -1), pos + glm::ivec3(1,  1, 0), pos + glm::ivec3(1,  1, -1)),
                            vertexAO(pos + glm::ivec3(1, 0,  1), pos + glm::ivec3(1,  1, 0), pos + glm::ivec3(1,  1,  1)),
                        };

                        glm::vec2 block_texcoord0[] = {
                            block.getTexCoord() + glm::vec2(0.0f,  0.0f),
                            block.getTexCoord() + glm::vec2(0.0f,  1.0f),
                            block.getTexCoord() + glm::vec2(0.25f, 0.0f),
                            block.getTexCoord() + glm::vec2(0.25f, 1.0f)
                        };

                        Vertex block_vertices[] = {
                            { block_pos[0], block_normal[0], block_ao[0], 1.0f, block_texcoord0[0], block_texcoord1 },
                            { block_pos[1], block_normal[1], block_ao[1], 1.0f, block_texcoord0[1], block_texcoord1 },
                            { block_pos[2], block_normal[2], block_ao[2], 1.0f, block_texcoord0[2], block_texcoord1 },
                            { block_pos[3], block_normal[3], block_ao[3], 1.0f, block_texcoord0[3], block_texcoord1 }
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
                        glm::vec3 block_pos[] = {
                            glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f),
                            glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f),
                            glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f),
                            glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f)
                        };

                        glm::vec3 block_normal[] = {
                            glm::vec3(-1.0f,  0.0f,  0.0f),
                            glm::vec3(-1.0f,  0.0f,  0.0f),
                            glm::vec3(-1.0f,  0.0f,  0.0f),
                            glm::vec3(-1.0f,  0.0f,  0.0f)
                        };

                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(-1, 0, -1), pos + glm::ivec3(-1, -1, 0), pos + glm::ivec3(-1, -1, -1)),
                            vertexAO(pos + glm::ivec3(-1, 0,  1), pos + glm::ivec3(-1, -1, 0), pos + glm::ivec3(-1, -1,  1)),
                            vertexAO(pos + glm::ivec3(-1, 0, -1), pos + glm::ivec3(-1,  1, 0), pos + glm::ivec3(-1,  1, -1)),
                            vertexAO(pos + glm::ivec3(-1, 0,  1), pos + glm::ivec3(-1,  1, 0), pos + glm::ivec3(-1,  1,  1)),
                        };
                        
                        glm::vec2 block_texcoord0[] = {
                            block.getTexCoord() + glm::vec2(0.0f,  0.0f), 
                            block.getTexCoord() + glm::vec2(0.0f,  1.0f), 
                            block.getTexCoord() + glm::vec2(0.25f, 0.0f), 
                            block.getTexCoord() + glm::vec2(0.25f, 1.0f) 
                        };

                        Vertex block_vertices[] = {
                            { block_pos[0], block_normal[0], block_ao[0], 1.0f, block_texcoord0[0], block_texcoord1 },
                            { block_pos[1], block_normal[1], block_ao[1], 1.0f, block_texcoord0[1], block_texcoord1 },
                            { block_pos[2], block_normal[2], block_ao[2], 1.0f, block_texcoord0[2], block_texcoord1 },
                            { block_pos[3], block_normal[3], block_ao[3], 1.0f, block_texcoord0[3], block_texcoord1 }
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
                        glm::vec3 block_pos[] = {
                            glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f),
                            glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f),
                            glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f),
                            glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f)
                        };

                        glm::vec3 block_normal[] = {
                            glm::vec3( 0.0f,  1.0f,  0.0f),
                            glm::vec3( 0.0f,  1.0f,  0.0f),
                            glm::vec3( 0.0f,  1.0f,  0.0f),
                            glm::vec3( 0.0f,  1.0f,  0.0f)
                        };

                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(0,  1, -1), pos + glm::ivec3(-1, 1, 0), pos + glm::ivec3(-1, 1, -1)),
                            vertexAO(pos + glm::ivec3(0,  1,  1), pos + glm::ivec3(-1, 1, 0), pos + glm::ivec3(-1, 1,  1)),
                            vertexAO(pos + glm::ivec3(0,  1, -1), pos + glm::ivec3( 1, 1, 0), pos + glm::ivec3( 1, 1, -1)),
                            vertexAO(pos + glm::ivec3(0,  1,  1), pos + glm::ivec3( 1, 1, 0), pos + glm::ivec3( 1, 1,  1)),
                        };

                        glm::vec2 block_texcoord0[] = {
                            block.getTexCoord() + glm::vec2(0.0f,  0.0f),
                            block.getTexCoord() + glm::vec2(0.0f,  1.0f),
                            block.getTexCoord() + glm::vec2(0.25f, 0.0f),
                            block.getTexCoord() + glm::vec2(0.25f, 1.0f)
                        };

                        Vertex block_vertices[] = {
                            { block_pos[0], block_normal[0], block_ao[0], height, block_texcoord0[0], block_texcoord1 },
                            { block_pos[1], block_normal[1], block_ao[1], height, block_texcoord0[1], block_texcoord1 },
                            { block_pos[2], block_normal[2], block_ao[2], height, block_texcoord0[2], block_texcoord1 },
                            { block_pos[3], block_normal[3], block_ao[3], height, block_texcoord0[3], block_texcoord1 }
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
                        glm::vec3 block_pos[] = {
                            glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f),
                            glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f),
                            glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f),
                            glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f)
                        };

                        glm::vec3 block_normal[] = {
                            glm::vec3( 0.0f, -1.0f,  0.0f),
                            glm::vec3( 0.0f, -1.0f,  0.0f),
                            glm::vec3( 0.0f, -1.0f,  0.0f),
                            glm::vec3( 0.0f, -1.0f,  0.0f)
                        };

                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(0, -1, -1), pos + glm::ivec3(-1, -1, 0), pos + glm::ivec3(-1, -1, -1)),
                            vertexAO(pos + glm::ivec3(0, -1,  1), pos + glm::ivec3(-1, -1, 0), pos + glm::ivec3(-1, -1,  1)),
                            vertexAO(pos + glm::ivec3(0, -1, -1), pos + glm::ivec3( 1, -1, 0), pos + glm::ivec3( 1, -1, -1)),
                            vertexAO(pos + glm::ivec3(0, -1,  1), pos + glm::ivec3( 1, -1, 0), pos + glm::ivec3( 1, -1,  1)),
                        };

                        glm::vec2 block_texcoord0[] = {
                            block.getTexCoord() + glm::vec2(0.0f,  0.0f),
                            block.getTexCoord() + glm::vec2(0.0f,  1.0f),
                            block.getTexCoord() + glm::vec2(0.25f, 0.0f),
                            block.getTexCoord() + glm::vec2(0.25f, 1.0f)
                        };

                        Vertex block_vertices[] = {
                            { block_pos[0], block_normal[0], block_ao[0], 1.0f, block_texcoord0[0], block_texcoord1 },
                            { block_pos[1], block_normal[1], block_ao[1], 1.0f, block_texcoord0[1], block_texcoord1 },
                            { block_pos[2], block_normal[2], block_ao[2], 1.0f, block_texcoord0[2], block_texcoord1 },
                            { block_pos[3], block_normal[3], block_ao[3], 1.0f, block_texcoord0[3], block_texcoord1 }
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
                        glm::vec3 block_pos[] = { 
                            glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f),
                            glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f),
                            glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f),
                            glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f)
                        };

                        glm::vec3 block_normal[] = {
                            glm::vec3( 0.0f,  0.0f,  1.0f),
                            glm::vec3( 0.0f,  0.0f,  1.0f),
                            glm::vec3( 0.0f,  0.0f,  1.0f),
                            glm::vec3( 0.0f,  0.0f,  1.0f)
                        };
                        
                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(0, -1, 1), pos + glm::ivec3(-1, 0, 1), pos + glm::ivec3(-1, -1, 1)),
                            vertexAO(pos + glm::ivec3(0,  1, 1), pos + glm::ivec3(-1, 0, 1), pos + glm::ivec3(-1,  1, 1)),
                            vertexAO(pos + glm::ivec3(0, -1, 1), pos + glm::ivec3( 1, 0, 1), pos + glm::ivec3( 1, -1, 1)),
                            vertexAO(pos + glm::ivec3(0,  1, 1), pos + glm::ivec3( 1, 0, 1), pos + glm::ivec3( 1,  1, 1)),
                        };

                        glm::vec2 block_texcoord0[] = {
                            block.getTexCoord() + glm::vec2(0.0f,  0.0f),
                            block.getTexCoord() + glm::vec2(0.0f,  1.0f),
                            block.getTexCoord() + glm::vec2(0.25f, 0.0f),
                            block.getTexCoord() + glm::vec2(0.25f, 1.0f)
                        };

                        Vertex block_vertices[] = {
                            { block_pos[0], block_normal[0], block_ao[0], 1.0f, block_texcoord0[0], block_texcoord1 },
                            { block_pos[1], block_normal[1], block_ao[1], 1.0f, block_texcoord0[1], block_texcoord1 },
                            { block_pos[2], block_normal[2], block_ao[2], 1.0f, block_texcoord0[2], block_texcoord1 },
                            { block_pos[3], block_normal[3], block_ao[3], 1.0f, block_texcoord0[3], block_texcoord1 }
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
                        glm::vec3 block_pos[] = {
                            glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f),
                            glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f),
                            glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f),
                            glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f)
                        };

                        glm::vec3 block_normal[] = {
                            glm::vec3( 0.0f,  0.0f, -1.0f),
                            glm::vec3( 0.0f,  0.0f, -1.0f),
                            glm::vec3( 0.0f,  0.0f, -1.0f),
                            glm::vec3( 0.0f,  0.0f, -1.0f)
                        };

                        float block_ao[] = {
                            vertexAO(pos + glm::ivec3(0, -1, -1), pos + glm::ivec3(-1, 0, -1), pos + glm::ivec3(-1, -1, -1)),
                            vertexAO(pos + glm::ivec3(0,  1, -1), pos + glm::ivec3(-1, 0, -1), pos + glm::ivec3(-1,  1, -1)),
                            vertexAO(pos + glm::ivec3(0, -1, -1), pos + glm::ivec3( 1, 0, -1), pos + glm::ivec3( 1, -1, -1)),
                            vertexAO(pos + glm::ivec3(0,  1, -1), pos + glm::ivec3( 1, 0, -1), pos + glm::ivec3( 1,  1, -1))
                        };

                        glm::vec2 block_texcoord0[] = {
                            block.getTexCoord() + glm::vec2(0.0f,  0.0f),
                            block.getTexCoord() + glm::vec2(0.0f,  1.0f),
                            block.getTexCoord() + glm::vec2(0.25f, 0.0f),
                            block.getTexCoord() + glm::vec2(0.25f, 1.0f)
                        };

                        Vertex block_vertices[] = {
                            { block_pos[0], block_normal[0], block_ao[0], 1.0f, block_texcoord0[0], block_texcoord1 },
                            { block_pos[1], block_normal[1], block_ao[1], 1.0f, block_texcoord0[1], block_texcoord1 },
                            { block_pos[2], block_normal[2], block_ao[2], 1.0f, block_texcoord0[2], block_texcoord1 },
                            { block_pos[3], block_normal[3], block_ao[3], 1.0f, block_texcoord0[3], block_texcoord1 }
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
    bgfx::updateTexture2D(heightmap_, 0, 0, 0, 0, kVolume.x, kVolume.z, bgfx::copy(heightmap, sizeof(heightmap)));
    dirty_ = false;
}

void Chunk::render(PerspectiveCamera cam) const {
    float fog[4] = { 
        Config::kViewDistance * kVolume.x - 12.0f, 
        Config::kViewDistance * kVolume.x - 2.0f, 
        0.0f, 0.97f 
    };
    auto view_pos = cam.pos;
    bgfx::setUniform(u_fog_, fog);
    bgfx::setTexture(0, s_atlas_, atlas_);
    bgfx::setTexture(1, s_heightmap_, heightmap_);
    bgfx::setViewTransform(0, glm::value_ptr(cam.view), glm::value_ptr(cam.proj));
    bgfx::setState(
        BGFX_STATE_WRITE_RGB       |
        BGFX_STATE_WRITE_A         |
        BGFX_STATE_WRITE_Z         | 
        BGFX_STATE_DEPTH_TEST_LESS |
        BGFX_STATE_CULL_CW |
        BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
    );
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

bool Chunk::inbound(PerspectiveCamera cam) const {
    auto min = World::getPosition(region_, glm::ivec3(0));
    auto max = World::getPosition(region_, glm::ivec3(kVolume - 1));
    glm::vec4 corners[] = {
        glm::vec4(min.x, min.y, min.z, 1.0f),
        glm::vec4(min.x, min.y, max.z, 1.0f),
        glm::vec4(min.x, max.y, min.z, 1.0f),
        glm::vec4(min.x, max.y, max.z, 1.0f),
        glm::vec4(max.x, min.y, min.z, 1.0f),
        glm::vec4(max.x, min.y, max.z, 1.0f),
        glm::vec4(max.x, max.y, min.z, 1.0f),
        glm::vec4(max.x, max.y, max.z, 1.0f),
    };
    bool right = true, left = true;
    bool top = true, bottom = true;
    bool back = true, front = true;
    for (auto corner : corners) {
        corner = cam.proj * cam.view * corner;

        right  &= corner.x >  corner.w;
        left   &= corner.x < -corner.w;
        top    &= corner.y >  corner.w;
        bottom &= corner.y < -corner.w;
        back   &= corner.z >  corner.w;
        if (bgfx::getCaps()->homogeneousDepth)
            front &= corner.z < -corner.w;
        else
            front &= corner.z < 0;
    }
    return !right && !left &&
           !top && !bottom &&
           !back && !front;
}