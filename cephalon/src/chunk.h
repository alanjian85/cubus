#ifndef CEPHALON_CHUNK_H_
#define CEPHALON_CHUNK_H_

#include <cassert>
#include <cstdint>
#include <vector>

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include "blocks/block.h"
#include "utils.h"

namespace cephalon {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        float ao;
        glm::vec2 tex_coord;
    };

    class World;

    class Chunk {
    public:
        static void init();
        
        static void cleanup();

        static constexpr glm::ivec3 kVolume = glm::ivec3(16, 256, 16);

        Chunk();

        Chunk(const Chunk&) = delete;

        Chunk& operator=(const Chunk&) = delete;

        ~Chunk() noexcept;

        void setBlock(glm::ivec3 offset, const Block& block);

        const Block& getBlock(glm::ivec3 offset) const;

        bool isDirty() const {
            return dirty_;
        }

        void rebuild(World& world, glm::ivec2 region);

        void render();
    private:
        static float vertexAO(const Block& side1, const Block& side2, const Block& corner);

        static bgfx::VertexLayout layout_;
        static bgfx::ProgramHandle program_;
        static bgfx::TextureHandle atlas_;
        static bgfx::UniformHandle s_atlas_;

        bool dirty_;
        const Block* blocks_[kVolume.x][kVolume.y][kVolume.z];

        bgfx::DynamicVertexBufferHandle vertex_buffer_;
        bgfx::DynamicIndexBufferHandle index_buffer_;
    };
}

#endif // CEPHALON_CHUNK_H_