#ifndef CEPHALON_UTILS_ASSETS_H_
#define CEPHALON_UTILS_ASSETS_H_

#include <algorithm>
#include <cstdint>

#include <bgfx/bgfx.h>
#include <bimg/decode.h>

namespace cephalon {
    bgfx::ShaderHandle LoadShader(const char* name);

    bgfx::ProgramHandle LoadProgram(const char* vs, const char* fs);

    bimg::ImageContainer* LoadImage(const char* name, bimg::TextureFormat::Enum dst_format = bimg::TextureFormat::Count);

    bgfx::TextureHandle LoadTexture(const char* name, std::uint64_t flags = 0);
}

#endif // CEPHALON_BGFX_UTILS_ASSETS_H_