#ifndef CEPHALON_UTILS_H_
#define CEPHALON_UTILS_H_

#include <cstdlib>
#include <functional>

#include <bgfx/bgfx.h>

namespace cephalon {
    struct Position {
        int x;
        int y;
        int z;
    };

    bgfx::ShaderHandle LoadShader(const char* name);

    bgfx::ProgramHandle LoadProgram(const char* vs, const char* fs);
}

namespace std {
    template <>
    struct hash<cephalon::Position> {
        size_t operator()(cephalon::Position pos) const {
            return ((hash<int>()(pos.x) 
                ^ (hash<int>()(pos.y) << 1)) >> 1)
                ^ (hash<int>()(pos.z) << 1);
        }
    };
}

#endif // CEPHALON_BGFX_UTILS_H_