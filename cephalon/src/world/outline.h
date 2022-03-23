#ifndef CEPHALON_WORLD_OUTLINE_H_
#define CEPHALON_WORLD_OUTLINE_H_

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include "utils/aabb.h"

namespace cephalon {
    class Outline {
    public:
        Outline();

        Outline(const Outline&) = delete;

        Outline& operator=(const Outline&) = delete;

        ~Outline();

        void update(glm::ivec3 pos, Direction dir);

        void render();
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

#endif // CEPHALON_WORLD_OUTLINE_H_