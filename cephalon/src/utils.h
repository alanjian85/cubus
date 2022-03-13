#ifndef CEPHALON_UTILS_H_
#define CEPHALON_UTILS_H_

#include <cstdlib>
#include <functional>

#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace cephalon {
    inline float get(bx::Vec3 vec, int index) {
        return index == 0 ? vec.x : index == 1 ? vec.y : vec.z;
    }

    struct Vec3i {
        Vec3i() = default;

        constexpr Vec3i(int i)
            : x(i), y(i), z(i)
        {

        }

        constexpr Vec3i(int x_, int y_, int z_)
            : x(x_), y(y_), z(z_)
        {

        }

        Vec3i(bx::Vec3 vec) {
            x = vec.x;
            y = vec.y;
            z = vec.z;
        }

        int operator[](int i) const {
            return i == 0 ? x : i == 1 ? y : z;
        }

        int x;
        int y;
        int z;            
    };

    inline bool operator==(Vec3i lhs, Vec3i rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }

    inline Vec3i operator+(Vec3i lhs, Vec3i rhs) {
        return Vec3i(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }

    inline Vec3i operator-(Vec3i lhs, Vec3i rhs) {
        return Vec3i(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }

    inline Vec3i operator*(Vec3i lhs, Vec3i rhs) {
        return Vec3i(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
    }

    inline Vec3i operator%(Vec3i lhs, Vec3i rhs) {
        return Vec3i(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z);
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