#ifndef CEPHALON_CAMERA_H_
#define CEPHALON_CAMERA_H_

#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace cephalon {
    struct PerspectiveCamera {
        void update() {
            dir = bx::Vec3(
                bx::cos(bx::toRad(yaw)) * bx::cos(bx::toRad(pitch)),
                bx::sin(bx::toRad(pitch)),
                bx::sin(bx::toRad(yaw)) * bx::cos(bx::toRad(pitch))
            );
            right = bx::normalize(bx::cross(bx::Vec3(0.0f, 1.0f, 0.0f), dir));
            bx::mtxLookAt(view, pos, bx::add(pos, dir));
            bx::mtxProj(proj, fov, aspect, near, far, bgfx::getCaps()->homogeneousDepth);
        }

        bx::Vec3 pos { 0.0f, 0.0f, 0.0f };

        float yaw = 90.0f;
        float pitch = 0.0f;

        float fov = 35.0f;
        float aspect;
        float near = 0.1f;
        float far = 1000.0f;

        bx::Vec3 dir { 0.0f };
        bx::Vec3 right { 0.0f };
        float view[16];
        float proj[16];
    };
}

#endif // CEPHALON_CAMERA_H_