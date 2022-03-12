#ifndef CEPHALON_CHUNK_H_
#define CEPHALON_CHUNK_H_

#include <cassert>
#include <cstdint>
#include <vector>

#include <bgfx/bgfx.h>
#include <bx/math.h>

#include "blocks/block.h"
#include "utils.h"

namespace cephalon {
    struct Vertex {
        static bgfx::VertexLayout layout;

        static void init() {
            layout.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
        }

        float x;
        float y;
        float z;
        std::uint32_t agbr;
    };

    class Chunk {
    public:
        static constexpr Vec3i kChunkSize = { 16, 16, 16 };

        Chunk(Vec3i pos = { 0, 0, 0 });

        Chunk(const Chunk&) = delete;

        Chunk& operator=(const Chunk&) = delete;

        void setBlock(Vec3i pos, const Block& block);

        const Block& getBlock(Vec3i pos) const;

        auto getVertexBuffer() const {
            return vertex_buffer_;
        }

        auto getIndexBuffer() const {
            return index_buffer_;
        }

        void update();

        ~Chunk() {
            bgfx::destroy(vertex_buffer_);
            bgfx::destroy(index_buffer_);
        }
    private:
        bool dirty_;
        std::vector<const Block*> blocks_;

        bgfx::DynamicVertexBufferHandle vertex_buffer_;
        bgfx::DynamicIndexBufferHandle index_buffer_;
    };
}

#endif // CEPHALON_CHUNK_H_