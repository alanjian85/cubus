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
        static constexpr int kChunkSizeX = 16;
        static constexpr int kChunkSizeY = 16;
        static constexpr int kChunkSizeZ = 16;

        Chunk(Position pos = { 0, 0, 0 });

        Chunk(const Chunk&) = delete;

        Chunk& operator=(const Chunk&) = delete;

        void setPos(Position pos);

        void setBlock(Position pos, const Block& block);

        const Block& getBlock(Position pos) const;

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
        Position pos_;

        bool dirty_;
        std::vector<const Block*> blocks_;

        bgfx::DynamicVertexBufferHandle vertex_buffer_;
        bgfx::DynamicIndexBufferHandle index_buffer_;
    };
}

#endif // CEPHALON_CHUNK_H_