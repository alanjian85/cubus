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

        bx::Vec3 position;
        std::uint32_t color;
    };

    class Chunk {
    public:
        static constexpr int kChunkSizeX = 16;
        static constexpr int kChunkSizeY = 16;
        static constexpr int kChunkSizeZ = 16;

        Chunk(int x = 0, int y = 0, int z = 0)
        {
            x_ = x;
            y_ = y;
            z_ = z;
            blocks_.resize(kChunkSizeX * kChunkSizeY * kChunkSizeZ, &blocks::kAir);
        }

        void setPosition(int x, int y, int z) {
            x_ = x;
            y_ = y;
            z_ = z;
        }

        void setBlock(int x, int y, int z, const Block& block) {
            x -= x_;
            y -= y_;
            z -= z_;
            assert(x >= 0 && x < kChunkSizeX);
            assert(y >= 0 && y < kChunkSizeY);
            assert(z >= 0 && z < kChunkSizeZ);
            blocks_[z * kChunkSizeX * kChunkSizeY + y * kChunkSizeX + x] = &block;
        }

        const Block& getBlock(int x, int y, int z) const {
            x -= x_;
            y -= y_;
            z -= z_;
            assert(x >= 0 && x < kChunkSizeX);
            assert(y >= 0 && y < kChunkSizeY);
            assert(z >= 0 && z < kChunkSizeZ);
            return *blocks_[z * kChunkSizeX * kChunkSizeY + y * kChunkSizeX + x];
        }

        auto getVertexBuffer() const {
            return vbh_;
        }

        auto getIndexBuffer() const {
            return ibh_;
        }
    private:
        int x_;
        int y_;
        int z_;

        std::vector<const Block*> blocks_;

        bgfx::VertexBufferHandle vbh_;
        bgfx::IndexBufferHandle ibh_;
    };
}

#endif // CEPHALON_CHUNK_H_