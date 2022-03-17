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
        std::uint32_t agbr;
    };

    class World;

    class Chunk {
    public:
        static void init();
        
        static void cleanup();

        static constexpr glm::ivec3 kVolume = { 16, 256, 16 };

        Chunk();

        Chunk(const Chunk&) = delete;

        Chunk& operator=(const Chunk&) = delete;

        ~Chunk() noexcept;

        void setBlock(glm::ivec3 pos, const Block& block);

        const Block& getBlock(glm::ivec3 pos) const;

        bool isDirty() const {
            return dirty_;
        }

        void rebuild(World& world);

        void render();
    private:
        static float vertexAO(const Block& side1, const Block& side2, const Block& corner);

        static bgfx::VertexLayout layout_;
        static bgfx::ProgramHandle program_;

        bool dirty_;
        std::vector<const Block*> blocks_;

        bgfx::DynamicVertexBufferHandle vertex_buffer_;
        bgfx::DynamicIndexBufferHandle index_buffer_;
    };
}

#endif // CEPHALON_CHUNK_H_