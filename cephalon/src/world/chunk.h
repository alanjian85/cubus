#ifndef CEPHALON_WORLD_CHUNK_H_
#define CEPHALON_WORLD_CHUNK_H_

#include <cassert>
#include <cstdint>
#include <vector>

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include "blocks/block.h"
#include "utils/aabb.h"
#include "utils/assets.h"
#include "utils/camera.h"
#include "utils/ray.h"

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

        Chunk(World& world, glm::ivec2 region);

        Chunk(Chunk&& rhs) noexcept;

        Chunk& operator=(const Chunk&) = delete;

        ~Chunk() noexcept;

        glm::ivec2 getRegion() const {
            return region_;
        }

        void setBlock(glm::ivec3 offset, const Block& block);

        const Block& getBlock(glm::ivec3 offset) const;

        bool isDirty() const {
            return dirty_;
        }

        void rebuild();

        void render(PerspectiveCamera cam) const;

        bool intersect(Ray ray, float dmin, float dmax, Direction& dir, glm::ivec3& offset, float& dist) const;

        bool inbound(PerspectiveCamera cam) const;
    private:
        static bgfx::VertexLayout layout_;
        static bgfx::ProgramHandle program_;
        static bgfx::TextureHandle atlas_;
        static bgfx::UniformHandle u_fog_;
        static bgfx::UniformHandle s_atlas_;

        float vertexAO(glm::ivec3 side1, glm::ivec3 side2, glm::ivec3 corner) const;

        glm::ivec2 region_;
        World& world_;
        bool dirty_;
        const Block* blocks_[kVolume.x][kVolume.y][kVolume.z];
        bgfx::DynamicVertexBufferHandle vertex_buffer_;
        bgfx::DynamicIndexBufferHandle index_buffer_;
    };
}

#endif // CEPHALON_WORLD_CHUNK_H_