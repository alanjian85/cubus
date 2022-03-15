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

        void onCursorMove(float relative_x, float relative_y);

        void onMouseLeftClick();

        void render();

        ~Game();
    private:        
        bgfx::ProgramHandle focus_program_;
        bgfx::UniformHandle u_color_;

        bgfx::VertexLayout focus_layout_;
        bgfx::VertexBufferHandle focus_vertex_buffer_;
        bgfx::IndexBufferHandle focus_index_buffer_;

        bool focus_;
        Vec3i focus_pos_;

        World world_;
        PerspectiveCamera camera_;
    };
}

#endif // CEPHALON_GAME_H_