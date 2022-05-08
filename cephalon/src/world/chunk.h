#ifndef CEPHALON_WORLD_CHUNK_H_
#define CEPHALON_WORLD_CHUNK_H_

#include <atomic>
#include <cassert>
#include <cstdint>
#include <mutex>
#include <vector>

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include "blocks/block.h"
#include "utils/aabb.h"
#include "utils/assets.h"
#include "utils/camera.h"
#include "utils/ray.h"

namespace cephalon {
    class World;

    enum class NeighborChunk {
        kNone = 0,
        kLeft = 1 << 0,
        kRight = 1 << 1,
        kDown = 1 << 2,
        kUp = 1 << 3,
        kLowerLeft = 1 << 4,
        kUpperLeft = 1 << 5,
        kLowerRight = 1 << 6,
        kUpperRight = 1 << 7
    };

    NeighborChunk& operator|=(NeighborChunk& lhs, NeighborChunk rhs);

    NeighborChunk operator&(NeighborChunk lhs, NeighborChunk rhs);

    class Chunk {
    public:
        static constexpr glm::ivec3 kVolume{ 16, 256, 16 };

        static void init();
        
        static void deinit();

        Chunk(World& world, glm::ivec2 region);

        Chunk(Chunk&) = delete;

        Chunk& operator=(const Chunk&) = delete;

        ~Chunk() noexcept;

        glm::ivec2 getRegion() const {
            return region_;
        }

        void setDirty(bool dirty) {
            dirty_.store(dirty);
        }

        bool isDirty() const {
            return dirty_.load();

        }

        void setRebuilding(bool rebuilding) {
            rebuilding_.store(rebuilding);
        }

        bool isRebuilding() const {
            return rebuilding_.load();
        }

        NeighborChunk setBlock(glm::ivec3 offset, const Block& block);

        const Block& getBlock(glm::ivec3 offset) const;

        void rebuild();

        void render(PerspectiveCamera cam) const;

        bool intersect(Ray ray, float dmin, float dmax, Direction& dir, glm::ivec3& offset, float& dist) const;

        bool inbound(PerspectiveCamera cam) const;
    private:
        struct Vertex {
            glm::vec3 pos;
            glm::vec3 normal;
            float ao;
            float height;
            glm::vec3 texcoord0;
            glm::vec2 texcoord1;
        };

        static bgfx::VertexLayout layout_;
        static bgfx::ProgramHandle program_;
        static bgfx::UniformHandle u_fog_;
        static bgfx::UniformHandle s_texture_;
        static bgfx::UniformHandle s_heightmap_;

        float vertexAO(glm::ivec3 side1, glm::ivec3 side2, glm::ivec3 corner) const;

        World& world_;
        glm::ivec2 region_;
        std::atomic_bool dirty_;
        std::atomic_bool rebuilding_;
        std::vector<Vertex> vertices_;
        std::vector<std::uint16_t> indices_;
        std::uint8_t heightmap_data_[kVolume.x][kVolume.z];

        mutable std::mutex mutex_;
        const Block* blocks_[kVolume.x * kVolume.y * kVolume.z];
        std::uint32_t num_vertices_;
        std::uint32_t num_indices_;

        bgfx::DynamicVertexBufferHandle vertex_buffer_;
        bgfx::DynamicIndexBufferHandle index_buffer_;
        bgfx::TextureHandle heightmap_;
    };
}

#endif // CEPHALON_WORLD_CHUNK_H_