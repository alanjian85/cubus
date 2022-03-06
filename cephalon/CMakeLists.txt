configure_file(cephalon_config.h.in cephalon_config.h)

set(CEPHALON_SRCS
    src/main.cc
    src/game.h)
add_executable(cephalon ${CEPHALON_SRCS})

find_package(SDL2 REQUIRED)
find_package(bgfx REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(cephalon
    ${SDL2_LIBRARIES}
    bgfx::bgfx
    fmt::fmt
)

target_include_directories(cephalon PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SDL2_INCLUDE_DIRS}
)