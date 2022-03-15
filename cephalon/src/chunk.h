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
        bx::Vec3 pos;
        std::uint32_t agbr;
    };

    class Chunk {
    public:
        static void init();
        
        static void cleanup();

        static constexpr Vec3i kChunkSize = { 16, 256, 16 };

        Chunk();

        Chunk(const Chunk&) = delete;

        Chunk& operator=(const Chunk&) = delete;

        void setBlock(Vec3i pos, const Block& block);

        const Block& getBlock(Vec3i pos) const;

        bool isDirty() const {
            return dirty_;
        }

        void rebuild();

        void render();

        ~Chunk() {
            bgfx::destroy(vertex_buffer_);
            bgfx::destroy(index_buffer_);
        }
    private:
        static bgfx::VertexLayout layout_;
        static bgfx::ProgramHandle program_;

        bool dirty_;
        std::vector<const Block*> blocks_;

        bgfx::DynamicVertexBufferHandle vertex_buffer_;
        bgfx::DynamicIndexBufferHandle index_buffer_;
    };
}

#endif // CEPHALON_CHUNK_H_