#ifndef CEPHALON_INPUT_H_
#define CEPHALON_INPUT_H_

#include <SDL2/SDL.h>

namespace cephalon {
    enum class Key {
        kA = SDL_SCANCODE_A,
        kD = SDL_SCANCODE_D,
        kS = SDL_SCANCODE_S,
        kW = SDL_SCANCODE_W
    };

    class Input {
    public:
        static void init(SDL_Window* window) {
            window_ = window;
        }

        static void update() {
            keyboard_state_ = SDL_GetKeyboardState(nullptr);
            relative_x_ = 0;
            relative_y_ = 0;
        }

        static bool getKey(Key key) {
            return keyboard_state_[static_cast<SDL_Scancode>(key)];
        }

        static void setRelativeMouseX(int relative_x) {
            relative_x_ = relative_x;
        }

        static int getRelativeMouseX() {
            return relative_x_;
        }

        static void setRelativeMouseY(int relative_y) {
            relative_y_ = relative_y;
        }

        static int getRelativeMouseY() {
            return relative_y_;
        }
    private:
        static SDL_Window* window_;
        static const Uint8* keyboard_state_;
        static int relative_x_;
        static int relative_y_;
    };
}

#endif // CEPHALON_INPUT_H_