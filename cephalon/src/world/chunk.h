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
#include "utils/atlas.h"
#include "utils/camera.h"
#include "utils/ray.h"

namespace cephalon {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        float ao;
        float height;
        glm::vec2 texcoord0;
        glm::vec2 texcoord1;
    };

    class World;

    class Chunk {
    public:
        static constexpr glm::ivec3 kVolume{ 16, 256, 16 };

        static void init();
        
        static void deinit();

        static Atlas& getAtlas() {
            return atlas_;
        }

        Chunk(World& world, glm::ivec2 region);

        Chunk(Chunk&& rhs) noexcept;

        Chunk& operator=(const Chunk&) = delete;

        ~Chunk() noexcept;

        glm::ivec2 getRegion() const {
            std::lock_guard lock(data_mutex_);
            return region_;
        }

        void setBlock(glm::ivec3 offset, const Block& block);

        const Block& getBlock(glm::ivec3 offset) const;

        void setDirty(bool dirty) {
            std::lock_guard lock(data_mutex_);
            dirty_ = dirty;
        }

        bool isDirty() const {
            return dirty_.load();
        }

        void rebuild();

        void render(PerspectiveCamera cam) const;

        bool intersect(Ray ray, float dmin, float dmax, Direction& dir, glm::ivec3& offset, float& dist) const;

        bool inbound(PerspectiveCamera cam) const;
    private:
        static bgfx::VertexLayout layout_;
        static bgfx::ProgramHandle program_;
        static Atlas atlas_;
        static bgfx::UniformHandle u_fog_;
        static bgfx::UniformHandle s_atlas_;
        static bgfx::UniformHandle s_heightmap_;

        float vertexAO(glm::ivec3 side1, glm::ivec3 side2, glm::ivec3 corner) const;

        std::atomic_bool dirty_;

        mutable std::mutex data_mutex_;
        glm::ivec2 region_;
        World& world_;
        const Block* blocks_[kVolume.x][kVolume.y][kVolume.z];

        mutable std::mutex buffer_mutex_;
        std::vector<Vertex> vertices_;
        std::vector<std::uint16_t> indices_;
        std::uint8_t heightmap_data_[kVolume.x][kVolume.z];

        bgfx::DynamicVertexBufferHandle vertex_buffer_;
        bgfx::DynamicIndexBufferHandle index_buffer_;
        bgfx::TextureHandle heightmap_;
    };
}

#endif // CEPHALON_WORLD_CHUNK_H_