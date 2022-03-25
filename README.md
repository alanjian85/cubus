# Cephalon
A Minecraft-like voxel game written in C++

## Feature
* Infinite World
* Block Destroy/Place
* Atlas Texture
* Different Types of Block
* Frustum Culling
* Shadow Mapping
* Ambient Occulusion
* Fog Effect

## Dependencies
* [bgfx](https://bkaradzic.github.io/bgfx/index.html)
* [{fmt}](https://github.com/fmtlib/fmt)
* [GLM](https://glm.g-truc.net/)
* [SDL2](https://www.libsdl.org/index.php)

## Installation
### Linux (Ubuntu/Debian)
Install bgfx
```
sudo apt install build-essential git cmake
sudo apt-get install libgl1-mesa-dev x11proto-core-dev libx11-dev
git clone https://github.com/bkaradzic/bgfx.cmake.git 
cd bgfx.cmake
mkdir build && cd build
cmake ..
sudo make install
```
Build Cephalon
```
sudo apt install libsdl2-dev libfmt-dev
git clone https://github.com/alanjian85/cephalon.git
cd cephalon
mkdir build && cd build
cmake ..
cmake --build .
```