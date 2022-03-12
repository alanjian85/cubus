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

        World world_;
        PerspectiveCamera camera_;
    };
}

#endif // CEPHALON_GAME_H_