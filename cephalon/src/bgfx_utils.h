#ifndef CEPHALON_BGFX_UTILS_H_
#define CEPHALON_BGFX_UTILS_H_

#include <bgfx/bgfx.h>

namespace cephalon {
    bgfx::ShaderHandle LoadShader(const char* name);

    bgfx::ProgramHandle LoadProgram(const char* vs, const char* fs);
}

#endif // CEPHALON_BGFX_UTILS_H_