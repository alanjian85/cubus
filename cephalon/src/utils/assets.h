#ifndef CEPHALON_UTILS_ASSETS_H_
#define CEPHALON_UTILS_ASSETS_H_

#include <cstdint>

#include <bgfx/bgfx.h>

namespace cephalon {
    bgfx::ShaderHandle LoadShader(const char* name);

    bgfx::ProgramHandle LoadProgram(const char* vs, const char* fs);

    bgfx::TextureHandle LoadTexture(const char* path, std::uint64_t flags = 0);
}

#endif // CEPHALON_BGFX_UTILS_ASSETS_H_