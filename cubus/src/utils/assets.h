#pragma once

#include <algorithm>
#include <cstdint>
#include <string>

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

namespace cephalon {
    bgfx::ShaderHandle LoadShader(const std::string& name);

    bgfx::ProgramHandle LoadProgram(const std::string& vs, const std::string& fs);

    bimg::ImageContainer* LoadImage(const std::string& name);
}
