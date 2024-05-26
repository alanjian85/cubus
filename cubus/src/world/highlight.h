#pragma once

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include "utils/aabb.h"
#include "utils/camera.h"

namespace cephalon {
    class Highlight {
    public:
        Highlight();

        Highlight(const Highlight&) = delete;

        Highlight& operator=(const Highlight&) = delete;

        ~Highlight();

        void update(glm::ivec3 pos, Direction dir);

        void render(PerspectiveCamera cam);
    private:
        bgfx::ProgramHandle program_;
        bgfx::VertexLayout layout_;
        bgfx::VertexBufferHandle vertex_buffer_;
        bgfx::IndexBufferHandle index_buffer_;

        bgfx::UniformHandle u_color_;
        std::uint32_t first_index_;
        glm::mat4 transform_;
    };
}
