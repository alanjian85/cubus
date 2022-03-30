#include "frustum.h"
using namespace cephalon;

Frustum::Frustum(PerspectiveCamera cam) {
    auto tan = glm::tan(glm::radians(cam.fov));
    auto height = cam.far * tan * 0.5f;
    auto width = cam.aspect * height;

    auto length_pos = glm::length(cam.pos);
    auto far = cam.dir * cam.far;
    auto near_center = cam.pos + cam.dir * cam.near;
    auto far_center = cam.pos + far;

    planes_[0] = Plane(glm::normalize(glm::cross(cam.up, far + cam.right * width)), length_pos); // right
    planes_[1] = Plane(glm::normalize(glm::cross(far - cam.right * width, cam.up)), length_pos); // left
    planes_[2] = Plane(glm::normalize(glm::cross(cam.right, far - cam.up * height)), length_pos); // top
    planes_[3] = Plane(glm::normalize(glm::cross(far + cam.up * height, cam.right)), length_pos); // bottom
    planes_[4] = Plane(-cam.dir, glm::length(far_center)); // far
    planes_[5] = Plane(cam.dir, glm::length(near_center)); // near
}