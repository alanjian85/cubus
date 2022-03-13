#ifndef CEPHALON_GAME_H_
#define CEPHALON_GAME_H_

#include <memory>

#include "camera.h"
#include "utils.h"
#include "world.h"

namespace cephalon {
    class Game {
    public:
        Game(int width, int height);

        void update(float delta);

        void render();

        ~Game();
    private:        
        bgfx::ProgramHandle chunks_program_;
        bgfx::ProgramHandle outline_program_;

        bgfx::VertexLayout outline_layout_;
        bgfx::VertexBufferHandle outline_vertex_buffer_;
        bgfx::IndexBufferHandle outline_index_buffer_;

        World world_;
        PerspectiveCamera camera_;

        bool intersect_;
        Vec3i lookat_pos_;
    };
}

#endif // CEPHALON_GAME_H_