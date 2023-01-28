# Cephalon
Cephalon is a voxel game similar to [Minecraft](https://www.minecraft.net/en-us) written in C++17. It simulates an infinite world using a procedural terrain generator that supports various types of terrain and includes a random seed option. The renderer used by cephalon is efficient through the use of optimal algorithms and multithreading. Furthermore, it is designed in such a way that new blocks and textures can be added without modifying unrelated code.

Cephalon is a ready-to-play game in which players can freely create and destroy blocks similar to the [creative mode](https://minecraft.fandom.com/wiki/Creative) in Minecraft. It also has a database subsystem that saves play records automatically when the game is closed and allows you to take screenshots by pressing F2 while playing.

![Cephalon GIF](docs/assets/cephalon.gif)

## Build Instructions
To build Cephalon, the following libraries and APIs are required:

* [bgfx](https://bkaradzic.github.io/bgfx/index.html)
* [Boost](https://www.boost.org/)
* [{fmt}](https://github.com/fmtlib/fmt)
* [GLM](https://glm.g-truc.net/)
* [JSON for Modern C++](https://json.nlohmann.me/)
* [SDL2](https://www.libsdl.org/index.php)
* [spdlog](https://github.com/gabime/spdlog)
* [SQLite3](https://www.sqlite.org/index.html)

The bgfx library should be built using [bgfx.cmake](https://github.com/bkaradzic/bgfx.cmake) to be compatible with Cephalon. The following instructions builds and installs bgfx and its CMake config files on [Arch Linux](https://archlinux.org/):

```sh
git clone https://github.com/bkaradzic/bgfx.cmake.git
cd bgfx.cmake
git submodule init
git submodule update
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build
```

After the dependencies listed above are installed, run the following instructions to build Cephalon:
```sh
mkdir build & cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

After build, the compiled binary can be found in the directory `build/cephalon`. The game should be run with the directory `assets` next to it. Finally, after the game is executed, a folder `save` that consists of a metadata JSON file and a SQLite database should be created.