#pragma once

#include <optional>

#include <glm/glm.hpp>

#include "ray.h"

namespace cephalon {
    enum class Direction {
        kRight,
        kLeft,
        kTop,
        kBottom,
        kBack,
        kFront
    };

    inline glm::ivec3 directionToVector(Direction dir) {
        switch (dir) {
            case Direction::kRight:
                return glm::ivec3( 1,  0,  0);
            case Direction::kLeft:
                return glm::ivec3(-1,  0,  0);
            case Direction::kTop:
                return glm::ivec3( 0,  1,  0);
            case Direction::kBottom:
                return glm::ivec3( 0, -1,  0);
            case Direction::kBack:
                return glm::ivec3( 0,  0,  1);
            case Direction::kFront:
                return glm::ivec3( 0,  0, -1);
        }
        return glm::ivec3(0, 0, 0);
    }

    struct AABB {
        AABB() = default;
        AABB(glm::vec3 min_, glm::vec3 max_)
            : min(min_), max(max_)
        {

        }

        bool intersect(Ray ray, float dmin, float dmax, Direction& dir, float dist) const {
            for (int i = 0; i < 3; ++i) {
                auto invd = 1.0f / ray.dir[i];
                auto t0 = (min[i] - ray.pos[i]) * invd;
                auto t1 = (max[i] - ray.pos[i]) * invd;
                if (invd < 0.0f)
                    std::swap(t0, t1);
                dmin = t0 > dmin ? t0 : dmin;
                dmax = t1 < dmax ? t1 : dmax;
                if (dmax <= dmin)
                    return false;
            }
            
            dist = dmin;
            auto point = ray.pos + ray.dir * dmin - (min + max) / 2.0f;
            if (std::abs(point.x) > std::abs(point.y) && std::abs(point.x) > std::abs(point.z)) {
                if (point.x > 0)
                    dir = Direction::kRight;
                else
                    dir = Direction::kLeft;
            }
            else if (std::abs(point.y) > std::abs(point.x) && std::abs(point.y) > std::abs(point.z)) {
                if (point.y > 0)
                    dir = Direction::kTop;
                else
                    dir = Direction::kBottom;
            }
            else {
                if (point.z > 0)
                    dir = Direction::kBack;
                else
                    dir = Direction::kFront;
            }
            return true;
        }

        glm::vec3 min;
        glm::vec3 max;
    };
}
