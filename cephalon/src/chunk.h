#ifndef CEPHALON_CHUNK_H_
#define CEPHALON_CHUNK_H_

#include <cassert>
#include <cstdint>
#include <vector>

#include <bgfx/bgfx.h>
#include <bx/math.h>

#include "blocks/block.h"

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
        std::uint32_t color;
    };

    class Chunk {
    public:
        static constexpr int kChunkSizeX = 16;
        static constexpr int kChunkSizeY = 16;
        static constexpr int kChunkSizeZ = 16;

        Chunk(int x = 0, int y = 0, int z = 0);

        Chunk(const Chunk&) = delete;

        Chunk& operator=(const Chunk&) = delete;

        void setPosition(int x, int y, int z);

        void setBlock(int x, int y, int z, const Block& block);

        const Block& getBlock(int x, int y, int z) const;

        auto getVertexBuffer() const {
            return vbh_;
        }

        auto getIndexBuffer() const {
            return ibh_;
        }

        void update();

        void destroy() {
            bgfx::destroy(vbh_);
            bgfx::destroy(ibh_);
        }
    private:
        int x_;
        int y_;
        int z_;

        bool dirty_;
        std::vector<const Block*> blocks_;

        bgfx::VertexBufferHandle vbh_;
        bgfx::IndexBufferHandle ibh_;
    };
}

#endif // CEPHALON_CHUNK_H_