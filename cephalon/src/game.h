#ifndef CEPHALON_GAME_H_
#define CEPHALON_GAME_H_

#include "camera.h"
#include "chunk.h"
#include "utils.h"

namespace cephalon {
    class Game {
    public:
        void init(int width, int height);

        void update(float delta);

        void render();
    private:
        bgfx::VertexLayout layout_;
        bgfx::VertexBufferHandle vbh_;

        bgfx::ProgramHandle program_;
    
        Chunk chunk_;

        PerspectiveCamera camera_;
    };
}

#endif // CEPHALON_GAME_H_