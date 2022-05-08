#include <filesystem>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <fmt/format.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <spdlog/spdlog.h>

#include "cephalon_config.h"
#include "game/callback.h"
#include "game/game.h"
#include "game/input.h"
#include "game/timer.h"
#include "utils/camera.h"
using namespace cephalon;

int main(int argc, char **argv) {
    int width = 1280;
    int height = 720;

    if (SDL_Init(0) < 0) {
        spdlog::error("Failed to initialize SDL");
    }
    SDL_Window* window = SDL_CreateWindow(
        fmt::format("Cephalon {}.{}.{}", CEPHALON_VERSION_MAJOR, CEPHALON_VERSION_MINOR, CEPHALON_VERSION_PATCH).c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        width, height,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        spdlog::error("Couldn't create window");
        SDL_Quit();
        return EXIT_FAILURE;
    }

    ALCdevice* device = alcOpenDevice(nullptr);
    ALCcontext* context = nullptr;
    if (device) {
        context = alcCreateContext(device, nullptr);
    } else {
        spdlog::error("Failed to open an OpenAL device");
        return EXIT_FAILURE;
    }

    if (context) {
        alcMakeContextCurrent(context);
    } else {
        spdlog::error("Failed to create OpenAL context");
        return EXIT_FAILURE;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(window, &wmi);

    Callback callback;
    bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = wmi.info.x11.display;
    init.platformData.nwh = reinterpret_cast<void*>(wmi.info.x11.window);
#elif BX_PLATFORM_OSX
    init.platformData.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = wmi.info.win.window;
#endif
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X4;
    init.callback = &callback;
	init.type = bgfx::RendererType::OpenGL;
	if (!bgfx::init(init)) {
        spdlog::error("Failed to initialize bgfx");
        SDL_Quit();
        return EXIT_FAILURE;
    }

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x87ceebff);
    bgfx::setViewRect(0, 0, 0, width, height);

    Config::init();
    Input::init(window);
    Chunk::init();
    Block::init();

    std::filesystem::create_directory("save");

    {
        Game game(width, height);
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
                        if (event.key.keysym.sym == SDLK_F2)
                            game.screenShot();
                        break;
                    case SDL_WINDOWEVENT:
                        switch (event.window.event) {
                            case SDL_WINDOWEVENT_RESIZED:
                                width = event.window.data1;
                                height = event.window.data2;
                                bgfx::reset(width, height, BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X4);
                                bgfx::setViewRect(0, 0, 0, width, height);
                                break;
                        }
                        break;
                    case SDL_MOUSEMOTION:
                        game.onCursorMove(event.motion.xrel, event.motion.yrel);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (event.button.button == SDL_BUTTON_LEFT)
                            game.onMouseLeftClick();
                        if (event.button.button == SDL_BUTTON_RIGHT)
                            game.onMouseRightClick();
                }
            }

            auto current_time = Timer::getTime();
            auto delta = current_time - last_time;
            last_time = current_time;

            game.update(delta);

            bgfx::touch(0);
            game.render();
            bgfx::frame();
        }
    }

    Block::deinit();
    Chunk::deinit();
    Config::deinit();
    bgfx::shutdown();
    alcDestroyContext(context);
    alcCloseDevice(device);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
