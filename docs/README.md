# Cubus
Cubus ([ˈkʊbʊs̠], a Latin word meaning "cube") is a voxel game resembling Minecraft. Featuring an expansive open-world sandbox environment, it simulates an infinite world full of cubes using a configurable procedural terrain generator. Cubus employs an efficient renderer, leveraging the chunk meshing algorithm and the potential of multithreading. Moreover, its design allows for seamless integration of new game elements like blocks and textures through the creation of corresponding classes.

In Cubus, players have the freedom to create and destroy blocks in the dynamically generated world. The game automatically saves progress upon exiting, and players can take screenshots by pressing F2.

<img src="cubus.gif" width="450" height="450">

## Build Instructions
To build Cubus, the following libraries are required (you can install them using the package manager of your operating system):

* [bgfx](https://bkaradzic.github.io/bgfx/index.html)
* [Boost](https://www.boost.org/)
* [{fmt}](https://github.com/fmtlib/fmt)
* [GLM](https://glm.g-truc.net/)
* [JSON for Modern C++](https://json.nlohmann.me/)
* [SDL2](https://www.libsdl.org/index.php)
* [spdlog](https://github.com/gabime/spdlog)
* [SQLite3](https://www.sqlite.org/index.html)

The bgfx library should be built using [bgfx.cmake](https://github.com/bkaradzic/bgfx.cmake) so that the build system of Cubus can detect it. Follow the instructions below to install bgfx.

```sh
git clone https://github.com/bkaradzic/bgfx.cmake.git
cd bgfx.cmake
git submodule init
git submodule update
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build
```

After the dependencies listed above are all installed, run the following instructions to build Cubus:
```sh
mkdir build & cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

The compiled binary can then be found in the directory `build/cubus`. You should run the binary of Cubus in that directory so that it can access the game assets. Finally, after the game is executed, a folder `save` consisting of a metadata JSON file and a SQLite database should be created.
