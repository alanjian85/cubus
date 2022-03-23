#ifndef CEPHALON_GAME_TIMER_H_
#define CEPHALON_GAME_TIMER_H_

#include <SDL2/SDL.h>

namespace cephalon {
    class Timer {
    public:
        static float getTime() {
            return SDL_GetTicks() / 1000.0f;
        }
    };
}

#endif // CEPHALON_GAME_TIMER_H_