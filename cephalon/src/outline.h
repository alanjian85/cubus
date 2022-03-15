#ifndef CEPHALON_OUTLINE_H_
#define CEPHALON_OUTLINE_H_

#include <bgfx/bgfx.h>

namespace cephalon {
    class Outline {
    private:
        bgfx::VertexBufferHandle vertex_buffer_;
        bgfx::IndexBufferHandle index_buffer_;
    };
}

#endif // CEPHALON_OUTLINE_H_