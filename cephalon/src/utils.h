#ifndef CEPHALON_UTILS_H_
#define CEPHALON_UTILS_H_

#include <cstdlib>
#include <functional>

#include <bgfx/bgfx.h>

namespace cephalon {
    struct Vec3i {
        int x;
        int y;
        int z;            
    };

    inline bool operator==(Vec3i lhs, Vec3i rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }

    bgfx::ShaderHandle LoadShader(const char* name);

    bgfx::ProgramHandle LoadProgram(const char* vs, const char* fs);
}

namespace std {
    template <>
    struct hash<cephalon::Vec3i> {
        size_t operator()(cephalon::Vec3i vec) const {
            return ((hash<int>()(vec.x) 
                ^ (hash<int>()(vec.y) << 1)) >> 1)
                ^ (hash<int>()(vec.z) << 1);
        }
    };
}

#endif // CEPHALON_BGFX_UTILS_H_