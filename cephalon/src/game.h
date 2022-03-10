#ifndef CEPHALON_GAME_H_
#define CEPHALON_GAME_H_

#include <memory>

#include "camera.h"
#include "chunk.h"
#include "utils.h"

namespace cephalon {
    class Game {
    public:
        void init(int width, int height);

        void update(float delta);

        void render();

        void cleanup();
    private:        
        bgfx::ProgramHandle chunks_program_;
    
        std::unique_ptr<Chunk> chunk_;

        PerspectiveCamera camera_;
    };
}

#endif // CEPHALON_GAME_H_