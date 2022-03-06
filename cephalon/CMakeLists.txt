set(CEPHALON_SRCS
    src/main.cc
    src/Game.h)
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
    ${SDL2_INCLUDE_DIRS}
)