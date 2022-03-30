#include "frustum.h"
using namespace cephalon;

Frustum::Frustum(PerspectiveCamera cam) {
    auto tan = glm::tan(glm::radians(cam.fov));
    auto near_height = cam.near * tan * 0.5f;
    auto near_width = cam.aspect * near_height;
    auto far_height = cam.far * tan * 0.5f;
    auto far_width = cam.aspect * far_height;

    auto near_center = cam.pos + cam.dir * cam.near;
    auto far_center = cam.pos + cam.dir * cam.far;

    planes_[0] = Plane(cam.dir, glm::length(near_center)); // near
    planes_[1] = Plane(cam.dir, glm::length(far_center)); // far
}