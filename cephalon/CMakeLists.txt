include(BgfxShader)

configure_file(cephalon_config.h.in cephalon_config.h)

set(CEPHALON_SRCS
    src/blocks/air.h
    src/blocks/block.h
    src/blocks/block.cc
    src/blocks/dirt.h
    src/blocks/grass.h
    src/blocks/stone.h
    src/camera.h
    src/chunk.h
    src/chunk.cc
    src/game.h
    src/game.cc
    src/input.cc
    src/input.h
    src/main.cc
    src/outline.h
    src/outline.cc
    src/utils.cc
    src/utils.h
    src/world.cc
    src/world.h
)
add_executable(cephalon ${CEPHALON_SRCS})

set(CEPHALON_SHADERS
    shaders/vs_chunks.sc
    shaders/fs_chunks.sc
    shaders/vs_outline.sc
    shaders/fs_outline.sc
)
add_bgfx_shader(cephalon ${CEPHALON_SHADERS})

find_package(SDL2 REQUIRED)
find_package(bgfx REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(cephalon
    ${SDL2_LIBRARIES}
    bgfx::bgfx
    fmt::fmt
)

target_include_directories(cephalon PRIVATE
    src
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SDL2_INCLUDE_DIRS}
)