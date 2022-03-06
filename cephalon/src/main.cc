#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <fmt/format.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "cephalon_config.h"
#include "game.h"
using namespace cephalon;

namespace {
    const int kWidth = 800;
    const int kHeight = 600;

    SDL_Window* window;
    Game game;
}

void init();
void mainLoop();
void cleanup();

int main(int argc, char **argv) {
    init();
    mainLoop();
    cleanup();
    return 0;
}

void init() {
    SDL_Init(0);

    window = SDL_CreateWindow(
        fmt::format("Cephalon {}.{}.{}", CEPHALON_VERSION_MAJOR, CEPHALON_VERSION_MINOR, CEPHALON_VERSION_PATCH).c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        kWidth, kHeight, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(window, &wmi);

    bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = wmi.info.x11.display;
    init.platformData.nwh = reinterpret_cast<void*>(wmi.info.x11.window);
#elif BX_PLATFORM_OSX
    init.platformData.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = wmi.info.win.window;
#endif
    init.resolution.width = kWidth;
    init.resolution.height = kHeight;
    init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::renderFrame();
    bgfx::init(init);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0x000000ff);
    bgfx::setViewRect(0, 0, 0, kWidth, kHeight);

    game.start();
}

void mainLoop() {
    bool quit = false;
    float lastTime = 0.0f;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            bgfx::reset(event.window.data1, event.window.data2, BGFX_RESET_VSYNC);
                            bgfx::setViewRect(0, 0, 0, event.window.data1, event.window.data2);
                            break;
                    }
                    break;
            }
        }

        float currentTime = SDL_GetTicks() / 1000.0f;
        float delta = currentTime - lastTime;
        lastTime = currentTime;
        game.update(delta);

        bgfx::touch(0);
        game.render();
        bgfx::frame();
    }
}

void cleanup() {
    bgfx::shutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();
}