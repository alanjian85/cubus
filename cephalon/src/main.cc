#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <fmt/format.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "camera.h"
#include "cephalon_config.h"
#include "game.h"
#include "input.h"
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

    SDL_SetRelativeMouseMode(SDL_TRUE);

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

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x334d4dff);
    bgfx::setViewRect(0, 0, 0, kWidth, kHeight);

    Input::init(window);
    Vertex::init();
    game.init(kWidth, kHeight);
}

void mainLoop() {
    bool quit = false;
    float last_time = 0.0f;
    while (!quit) {
        Input::update();

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
                case SDL_MOUSEMOTION:
                    Input::setRelativeMouseX(event.motion.xrel);
                    Input::setRelativeMouseY(event.motion.yrel);
                    break;
            }
        }

        float current_time = SDL_GetTicks() / 1000.0f;
        float delta = current_time - last_time;
        last_time = current_time;

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