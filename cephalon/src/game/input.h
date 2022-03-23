#ifndef CEPHALON_GAME_INPUT_H_
#define CEPHALON_GAME_INPUT_H_

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
        }

        static bool getKey(Key key) {
            return keyboard_state_[static_cast<SDL_Scancode>(key)];
        }
    private:
        static SDL_Window* window_;
        static const Uint8* keyboard_state_;
    };
}

#endif // CEPHALON_GAME_INPUT_H_