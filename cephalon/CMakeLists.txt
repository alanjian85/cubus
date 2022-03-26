include(Shader)
include(Texture)

configure_file(cephalon_config.h.in cephalon_config.h)

set(CEPHALON_SRCS
    src/blocks/air.h
    src/blocks/block.cc
    src/blocks/block.h
    src/blocks/dirt.h
    src/blocks/grass.h
    src/blocks/stone.h
    src/blocks/wood.h
    src/utils/aabb.h
    src/utils/assets.h
    src/utils/assets.cc
    src/utils/camera.h
    src/utils/ray.h
    src/world/chunk.h
    src/world/chunk.cc
    src/world/generator.h
    src/world/outline.h
    src/world/outline.cc
    src/world/world.cc
    src/world/world.h
    src/game/callback.h
    src/game/config.h
    src/game/config.cc
    src/game/game.cc
    src/game/game.h
    src/game/input.cc
    src/game/input.h
    src/game/timer.h
    src/main.cc
)
add_executable(cephalon ${CEPHALON_SRCS})

set(CEPHALON_SHADERS
    shaders/vs_chunks.sc
    shaders/fs_chunks.sc
    shaders/vs_outline.sc
    shaders/fs_outline.sc
)
add_shader(cephalon ${CEPHALON_SHADERS})

set(CEPHALON_TEXTURES
    textures/blocks/dirt.png
    textures/blocks/grass.png
    textures/blocks/stone.png
    textures/blocks/wood.png
)
add_texture(cephalon ${CEPHALON_TEXTURES})

find_package(bgfx REQUIRED)
find_package(fmt REQUIRED)
find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SDL2 REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(cephalon
    bgfx::bgfx
    fmt::fmt
    glm::glm
    ${SDL2_LIBRARIES}
    spdlog::spdlog
)

target_include_directories(cephalon PRIVATE
    src
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_definitions(cephalon PRIVATE
    GLM_FORCE_LEFT_HANDED
)