#ifndef CEPHALON_GAME_H_
#define CEPHALON_GAME_H_

#include <memory>

#include <glm/glm.hpp>

#include "camera.h"
#include "outline.h"
#include "utils.h"
#include "world.h"

namespace cephalon {
    class Game {
    public:
        Game(int width, int height);

        void update(float delta);

        void onCursorMove(float relative_x, float relative_y);

        void onMouseLeftClick();

        void onMouseRightClick();

        void render();
    private:
        bool intersect_;
        glm::ivec3 intersect_pos_;
        Direction intersect_dir_;
        Outline outline_;

        World world_;
        PerspectiveCamera camera_;
    };
}

#endif // CEPHALON_GAME_H_