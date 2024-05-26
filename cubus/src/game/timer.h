#pragma once

#include <SDL2/SDL.h>

namespace cephalon {
    class Timer {
    public:
        static float getTime() {
            return SDL_GetTicks() / 1000.0f;
        }
    };
}
