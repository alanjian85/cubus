#ifndef CEPHALON_OUTLINE_H_
#define CEPHALON_OUTLINE_H_

#include <bgfx/bgfx.h>

#include "utils.h"

namespace cephalon {
    class Outline {
    public:
        Outline();

        Outline(const Outline&) = delete;

        Outline& operator=(const Outline&) = delete;

        ~Outline();

        void update(Vec3i pos);

        void render();
    private:
        bgfx::ProgramHandle program_;
        bgfx::VertexLayout layout_;
        bgfx::VertexBufferHandle vertex_buffer_;
        bgfx::IndexBufferHandle index_buffer_;
        float transform_[16];
    };
}

#endif // CEPHALON_OUTLINE_H_