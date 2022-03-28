#ifndef CEPHALON_GAME_GAME_H_
#define CEPHALON_GAME_GAME_H_

#include <memory>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "utils/assets.h"
#include "utils/camera.h"
#include "world/outline.h"
#include "world/world.h"

namespace cephalon {
    class Game {
    public:
        Game(int width, int height);

        ~Game();

        void update(float delta);

        void onCursorMove(float relative_x, float relative_y);

        void onMouseLeftClick();

        void onMouseRightClick();

        void screenShot();

        void render();
    private:
        bool intersected_;
        glm::ivec3 destroy_pos_;
        glm::ivec3 place_pos_;
        Direction block_dir_;
        Outline outline_;

        nlohmann::json info_;
        World world_;
        PerspectiveCamera camera_;
    };
}

#endif // CEPHALON_GAME_GAME_H_