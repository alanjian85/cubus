#ifndef CEPHALON_BGFX_UTILS_H_
#define CEPHALON_BGFX_UTILS_H_

#include <bgfx/bgfx.h>

namespace cephalon {
    bgfx::ShaderHandle loadShader(const char* name);

    bgfx::ProgramHandle loadProgram(const char* vs, const char* fs);
}

#endif // CEPHALON_BGFX_UTILS_H_